#include "BeeThree.h"
#include "Clarinet.h"
#include "RtAudio.h"
#include <cmath>
#include <iostream>
using namespace stk;
// The TickData structure holds all the class instances and data that
// are shared by the various processing functions.
struct TickData {
	Instrmnt *instrument;
	StkFloat frequency;
	StkFloat scaler;
	long counter;
	bool done;
	// Default constructor.
	TickData()
		: instrument(0), scaler(1.0), counter(0), done( false ) {}
};


int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData );
int lets_sound(int *cam_variables);