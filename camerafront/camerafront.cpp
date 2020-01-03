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
	Mat bgframe, frame, prevframe, grayframe, satframe, fgMask, edges, colored;
	Mat frameH, frameS, frameV;
	cap >> bgframe;
	int k, cutx;
	while( (k = waitKey(20)) != 32){
		if(k == 115){ //S key to set background reference
			cap >> bgframe;
			GaussianBlur(bgframe,bgframe,Size( 5, 5 ),1.0,0.0);
		}
		cap >> frame;
		GaussianBlur(frame,frame,Size( 5, 5 ),1.0,0);
		pBackSub -> apply(frame,fgMask);
		//imshow("background subtraction",fgMask);
		//cvtColor(frame-bgframe,grayframe,COLOR_BGR2GRAY);
		
		cvtColor(frame, satframe,COLOR_BGR2HSV);
		vector<Mat> hsvarray(3);
		split(satframe, hsvarray);
		hsvarray[1] *= 1.5;
		merge(hsvarray, satframe);
		inRange(satframe, Scalar(90, 20, 100), Scalar(110, 200, 255), colored);
		fgMask = min(fgMask,colored);

		Mat Points, PointsL, PointsR;
		findNonZero(fgMask,Points);
		Rect Min_Rect=boundingRect(Points);

		if (countNonZero(fgMask.col((Min_Rect.tl().x + Min_Rect.br().x)/2))>1){
			cutx = (fgMask.cols + Min_Rect.tl().x + Min_Rect.br().x)/2 % fgMask.cols;
		}else{
			cutx = (Min_Rect.tl().x + Min_Rect.br().x)/2;
		}
		
		Mat fgMaskL(fgMask, Rect(0,0,cutx,fgMask.rows-1));
		findNonZero(fgMaskL,PointsL);
		Rect Min_Rect_L = boundingRect(PointsL);

		Mat fgMaskR(fgMask, Rect(cutx,0,fgMask.cols-1-cutx,fgMask.rows-1));
		findNonZero(fgMaskR,PointsR);
		Rect Min_Rect_R = boundingRect(PointsR)+ Point(cutx,0);

		rectangle(frame,Min_Rect,Scalar(0,255,0),2);
		rectangle(frame,Min_Rect_L,Scalar(255,0,0),2);
		rectangle(frame,Min_Rect_R,Scalar(0,0,255),2);
		rectangle(fgMask,Min_Rect,Scalar(255,255,255),2);
		line(fgMask,Point(cutx,0), Point(cutx,fgMask.rows-1), Scalar(255,255,255),2);
		//Canny(colored,edges,10.0,30.0,3);
		//GaussianBlur(fgMask,fgMask,Size( 5, 5 ),1.0,0);
		//cvtColor(satframe,satframe, COLOR_HSV2BGR);

		
		//imshow("satframe", satframe);
		//imshow("canny",edges);
		imshow("frame",frame);
		//imshow("fgMask",fgMask);
		imshow("colored",colored);
		//imshow("canny",edges);
	}
	return 0;
}
