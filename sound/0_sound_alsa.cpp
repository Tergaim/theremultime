
/*
 *  This extra small demo sends a random samples to your speakers.
 */

// compile with "g++ sound.cpp -lasound" 
#include <alsa/asoundlib.h>
#include <cmath>
#include <cstdlib>
static char *device = "default";            /* playback device */
snd_output_t *output = NULL;
unsigned char buffer[16*1024];              /* some random data */
int main(void)
{
	int err;
	unsigned int i;
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;
	int gamme[] = {261,294,330,349,392,440,493,523,0};
        
    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n");
        exit(EXIT_FAILURE);
    }
    if ((err = snd_pcm_set_params(handle,
                                  SND_PCM_FORMAT_U8,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  1,
                                  48000,
                                  1,
                                  500000)) < 0) {   /* 0.5sec */
        printf("Playback open error: %s\n");
        exit(EXIT_FAILURE);
    }
	int partoche[] = {7,7,5,7,7,5,7,7,5,6,6,8};
	for(int j : partoche) {
		for (i = 0; i < sizeof(buffer); i++)
			buffer[i] = std::sin(2*3.14*i/(gamme[7-j]))*100;
			buffer[i] += std::sin(10*3.14*i/(gamme[7-j]))*50;
		frames = snd_pcm_writei(handle, buffer, sizeof(buffer));
		if (frames < 0)
			frames = snd_pcm_recover(handle, frames, 0);
		if (frames < 0) {
			printf("snd_pcm_writei failed: %s\n");
			break;
		}
		if (frames > 0 && frames < (long)sizeof(buffer))
			printf("Short write (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
		//j = rand()%8;
		printf("j= %d \n",j);
	}
	for(int j : partoche) {
		for (i = 0; i < sizeof(buffer); i++)
			buffer[i] = std::sin(2*3.14*i/(gamme[7-j]))*100;
		frames = snd_pcm_writei(handle, buffer, sizeof(buffer));
		if (frames < 0)
			frames = snd_pcm_recover(handle, frames, 0);
		if (frames < 0) {
			printf("snd_pcm_writei failed: %s\n");
			break;
		}
		if (frames > 0 && frames < (long)sizeof(buffer))
			printf("Short write (expected %li, wrote %li)\n", (long)sizeof(buffer), frames);
		//j = rand()%8;
		printf("j= %d \n",j);
	}
    snd_pcm_close(handle);
    return 0;
}