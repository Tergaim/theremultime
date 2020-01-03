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
	int *cam_variables = (int *)malloc(2 * sizeof(int));;
	cam_variables[0] = 1000;
	cam_variables[1] = 1000;
	pthread_t camera;
	pthread_create(&camera, NULL, *front_cam_run, cam_variables);
	lets_sound(cam_variables);
	pthread_join(camera, NULL);

	return 0;
}
