#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

#include "camerafront/camerafront.hpp"
#include "sound/1_sound.hpp"
#include <pthread.h>

using namespace std;
using namespace cv;

int main() {
	// create shared variables
	int *cam_variables = (int *)malloc(6 * sizeof(int));
	for(int i=0; i<8; i++)
		cam_variables[i] = 100;

	// initiate camera thread
	pthread_t camera;
	pthread_create(&camera, NULL, *front_cam_run, cam_variables);

	// launch sound loop
	lets_sound(cam_variables);

	// wait for camera thread to end
	pthread_join(camera, NULL);

	return 0;
}
