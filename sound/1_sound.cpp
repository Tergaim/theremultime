#include "1_sound.hpp"
using namespace stk;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData )
{
	TickData *data = (TickData *) userData;
	register StkFloat *samples = (StkFloat *) outputBuffer;
	for ( unsigned int i=0; i<nBufferFrames; i++ ) {
		*samples++ = data->instrument->tick();
		if ( ++data->counter % 2000 == 0 ) {
			//data->scaler += 0.010;
			data->instrument->setFrequency( data->frequency * data->scaler );
		}
	}
	if ( data->counter > 2400 )
		data->done = true;
	return 0;
}
int lets_sound(int *params) {
	// Set the global sample rate and rawwave path before creating class instances.
	Stk::setSampleRate( 44100.0 );
	Stk::setRawwavePath( "../sound/rawwaves/" );
	TickData data;
	RtAudio dac = RtAudio();
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 1;
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	unsigned int bufferFrames = RT_BUFFER_SIZE;
	try {
		dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&data );
	}
	catch ( RtAudioError& error ) {
		error.printMessage();
		goto cleanup;
	}
	try {
		// Define and load the instrument
		data.instrument = new Clarinet(55);
	}
	catch ( StkError & ) {
		goto cleanup;
	}
	data.frequency = 261.0;
	data.instrument->noteOn( data.frequency, 1 );
	try {
		dac.startStream();
	}
	catch ( RtAudioError &error ) {
		error.printMessage();
		goto cleanup;
	}
	while(true) {
		// Get hand position using mean of last detections
		const int cam_variables[2] = {(params[0]+params[1]+params[2])/3, (params[3]+params[4]+params[5])/3};

		// Compute frequency using left hand according to the actual theremin formula, and keep it between A0 and A5
		data.frequency = 180 / pow((cam_variables[0]+1)/800., 0.6) - 150;
		data.frequency /= 4;
		data.frequency = (data.frequency < 55) ? 55 : data.frequency;
		data.frequency = (data.frequency > 1760) ? 1760 : data.frequency;
		// Compute volume using right hand
		float amplitude = 1 - cam_variables[1]/600.;
		amplitude = (amplitude > 1) ? 1 : amplitude;
		amplitude = (amplitude < 0) ? 0 : amplitude;

		// std::cout << "data.frequency = " << data.frequency << std::endl;
		// std::cout << "valeur cam = " << cam_variables[0] <<" " << cam_variables[1]<< std::endl;
		// std::cout << "amplitude = " << amplitude << std::endl;

		// If hands are on screen, play a sound
		if(amplitude && cam_variables[1] > 0 && cam_variables[0] > 0) {
			data.instrument->noteOn( data.frequency, amplitude );		
			// Block waiting until callback signals done.
			while ( !data.done )
				Stk::sleep( 20 );
			data.instrument -> noteOff(1);
		} else {
			Stk::sleep(20);
		}
		data.done = false;
		data.counter = 0;
	}
	// Shut down the callback and output stream.
	try {
		dac.closeStream();
	}
	catch ( RtAudioError &error ) {
		error.printMessage();
	}
	cleanup:
	delete data.instrument;
	return 0;
}
