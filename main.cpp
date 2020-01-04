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
	int *cam_variables = (int *)malloc(6 * sizeof(int));
	for(int i=0; i<6; i++)
		cam_variables[i] = 100;
	pthread_t camera;
	pthread_create(&camera, NULL, *front_cam_run, cam_variables);
	lets_sound(cam_variables);
	pthread_join(camera, NULL);

	return 0;
}
