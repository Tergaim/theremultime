#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;
using namespace cv;

int main() {
	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorKNN();

	VideoCapture cap(0);
	Mat frame, fgMask;
	while(waitKey(20) <= 0){
		cap >> frame;
		pBackSub -> apply(frame,fgMask);
		imshow("fgMask",fgMask);
		imshow("frame",frame);
	}
	return 0;
}
