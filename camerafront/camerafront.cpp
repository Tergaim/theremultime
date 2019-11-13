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
	//Ptr<bgsegm::BackgroundSubtractorGMG> pBackSub;
	//pBackSub = bgsegm::createBackgroundSubtractorGMG(120,0.8);
	//pBackSub.setUpdateBackgroundModel(false);

	VideoCapture cap(0);
	Mat bgframe, frame, fgMask, edges;
	Mat fgMaskR, fgMaskG, fgMaskB;
	cap >> bgframe;
	int k;
	while( (k = waitKey(20)) != 32){
		if(k == 115){ //S key to set background reference
			cap >> bgframe;
			GaussianBlur(bgframe,bgframe,Size( 5, 5 ),0,0);
			//normalize(bgframe,bgframe,0,1,NORM_MINMAX);
			//bgframe -= mean(bgframe);
			//bgframe = abs(bgframe);
		}
		cap >> frame;
		GaussianBlur(frame,frame,Size( 5, 5 ),0,0);
		//normalize(frame,frame,0,1,NORM_MINMAX);
		//frame -= mean(frame);
		//frame = abs(frame);
		extractChannel(frame-bgframe, fgMaskB, 0);
		extractChannel(frame-bgframe, fgMaskG, 1);
		extractChannel(frame-bgframe, fgMaskR, 2);

		threshold(fgMaskB,fgMaskB,30,255.0,THRESH_BINARY);
		threshold(fgMaskG,fgMaskG,30,255.0,THRESH_BINARY);
		threshold(fgMaskR,fgMaskR,30,255.0,THRESH_BINARY);
		//pBackSub -> apply(frame,fgMask);
		//GaussianBlur(fgMask,fgMask,Size( 25, 25 ),0,0);
		//Canny(fgMask,edges,10.0,30.0,3);
		//cvtColor(frame-bgframe,fgMask,COLOR_BGR2GRAY);
		fgMask = max(fgMaskR,max(fgMaskG,fgMaskB));
		imshow("fgMask",fgMask);
		//imshow("fgMaskR",fgMaskR);
		//imshow("fgMaskG",fgMaskG);
		//imshow("fgMaskB",fgMaskB);
		//imshow("canny",edges);
		imshow("frame",frame-bgframe);
	}
	return 0;
}
