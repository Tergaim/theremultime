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


using namespace std;
using namespace cv;

int main() {
	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorMOG2(500,8,false);
	//pBackSub.setUpdateBackgroundModel(false);

	VideoCapture cap(0);
	Mat bgframe, frame, prevframe, grayframe, satframe, fgMask, edges, gabored;
	Mat fgMaskR, fgMaskG, fgMaskB;
	Mat frameH, frameS, frameV;
	cap >> bgframe;
	int k;
	while( (k = waitKey(20)) != 32){
		if(k == 115){ //S key to set background reference
			cap >> bgframe;
			GaussianBlur(bgframe,bgframe,Size( 5, 5 ),1.0,0.0);
			//normalize(bgframe,bgframe,0,1,NORM_MINMAX);
			//bgframe -= mean(bgframe);
			//bgframe = abs(bgframe);
		}
		cap >> frame;
		GaussianBlur(frame,frame,Size( 5, 5 ),1.0,0);
		//normalize(frame,frame,0,1,NORM_MINMAX);
		//frame -= mean(frame);
		//frame = abs(frame);
		extractChannel(frame-bgframe, fgMaskB, 0);
		extractChannel(frame-bgframe, fgMaskG, 1);
		extractChannel(frame-bgframe, fgMaskR, 2);

		threshold(fgMaskB,fgMaskB,10,255.0,THRESH_BINARY);
		threshold(fgMaskG,fgMaskG,10,255.0,THRESH_BINARY);
		threshold(fgMaskR,fgMaskR,10,255.0,THRESH_BINARY);
		//pBackSub -> apply(frame,fgMask);
		//GaussianBlur(fgMask,fgMask,Size( 25, 25 ),0,0);
		cvtColor(frame-bgframe,grayframe,COLOR_BGR2GRAY);
		fgMask = max(fgMaskR,max(fgMaskG,fgMaskB));
		Canny(frame,edges,10.0,30.0,3);
		cvtColor(frame, satframe,COLOR_BGR2HSV);
		vector<Mat> hsvarray(3);
		split(satframe, hsvarray);
		hsvarray[1] *= 1.5;
		merge(hsvarray, satframe);
		//cvtColor(satframe,satframe, COLOR_HSV2BGR);
		imshow("satframe", satframe);
		//imshow("fgMaskR",fgMaskR);
		//imshow("fgMaskG",fgMaskG);
		//imshow("fgMaskB",fgMaskB);
		imshow("canny",edges);
		imshow("frame",frame);
		//flip(frame,frame,1);
		inRange(satframe, Scalar(90, 20, 0), Scalar(120, 200, 255), fgMask);
		inRange(hsvarray[0], 90, 120, frameH);
		inRange(hsvarray[1], 20, 200, frameS);
		inRange(hsvarray[2], 0, 255, frameV);
		//fgMask = min(min(frameH, frameS),frameV);
		//bitwise_and(frame,frame, frame, fgMask);
		imshow("fgMask",fgMask);
		//next time : try hysteresis thresholding to get only hands
		//Gabor filter for texture segmentation
		Mat kernel1;
		int kernel_size = 3;
        double sig = 5, th = 0, lm = 8, gm = 0.02, ps = 0, theta=0;
		kernel1 = cv::getGaborKernel(cv::Size(kernel_size,kernel_size), sig, theta, lm, gm, ps, CV_32F);
		//grayframe.convertTo(grayframe,CV_32F);
        //filter2D(frame, gabored, CV_32F, kernel1);
		//imshow("grey", grayframe);

	}
	return 0;
}
