#include <sstream>
#include <opencv2/bgsegm.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

void set_instrument(int &vertical, int &horizontal, cv::Mat &frame);

void* front_cam_run(void *cam_param);