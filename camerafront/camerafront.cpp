#include "camerafront.hpp"
#include <cstring>

using namespace std;
using namespace cv;

double saturation_rate = 1.5; //1.5
int H1=95, S1=50, V1=100, H2=105, S2=255, V2=255; //95,50,100,105,255,255
int openSize=2, closeSize=4; //2,4

void set_instrument(int &vertical, int &horizontal, Mat &frame) {
	int n_pass = 10;
	int n_begin = n_pass;
	float ratio[n_pass+1];
	Mat pattern[2], pattern_gray[2], frame_gray, resizedframe[n_pass+1];
	pattern[0] = imread("../pattern1.jpg");
	pattern[1] = imread("../pattern2.jpg");
	
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	Canny(frame_gray, frame_gray, 30, 90);
	for(int i = n_pass; i>0; i--) {
		ratio[i] = float(i)/n_pass;
		int newX = ratio[i] * frame.rows;
		int newY = ratio[i] * frame.cols;
		resize(frame_gray, resizedframe[i], Size(newY,newX));
		n_pass = i+1;
		if (newX < pattern[0].rows || newX < pattern[1].rows || newY < pattern[0].cols || newY < pattern[1].cols) {
			break;
		}
	}
	for(int i = 0; i<2; i++) {
		cvtColor(pattern[i], pattern_gray[i], COLOR_BGR2GRAY);
		Canny(pattern_gray[i], pattern[i], 30, 90);
	}
	int maxCoords[2][2];
	for(int j = 0; j<2; j++) {
		float maxValEver = 0;
		for(int i = n_begin; i>n_pass; i--){
			Mat result;
			int result_cols =  resizedframe[i].cols - pattern[j].cols + 1;
			int result_rows = resizedframe[i].rows - pattern[j].rows + 1;
			result.create(result_rows, result_cols, CV_32FC1);

			double minVal; double maxVal; Point minLoc; Point maxLoc;
			matchTemplate(resizedframe[i], pattern[j], result, 5);
			//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
			if(maxVal > maxValEver) {
				cout << 100*j+i << " " << maxVal << " " << ratio[i] << endl;
				maxValEver = maxVal;
				maxCoords[j][0] = maxLoc.x / ratio[i];
				maxCoords[j][1] = maxLoc.y / ratio[i];
			}
		}
	}
	maxCoords[0][0] += pattern[0].rows;
	maxCoords[0][1] += pattern[0].cols;
	maxCoords[1][1] += pattern[1].cols;	
	maxCoords[1][0] += pattern[1].rows;
	// Get y coordinate of upper left pattern
	vertical = (maxCoords[0][0] > maxCoords[1][0]) ? maxCoords[1][1] : maxCoords[0][1];
	// Get x coordinate of down right pattern
	horizontal = (maxCoords[0][1] > maxCoords[1][1]) ? maxCoords[1][0] : maxCoords[0][0];
	
	circle(frame, Point(maxCoords[0][0], maxCoords[0][1]), 3, Scalar(255,0,0));
	circle(frame, Point(maxCoords[1][0], maxCoords[1][1]), 3, Scalar(255,0,0));
	imshow("Theremultime Demo2",frame);
}

void setOpen(int, void*){
	if(openSize < 3){
		openSize = 3;
	}
}

void setClose(int, void*){
	if(closeSize < 3){
		closeSize = 3;
	}
}

void* front_cam_run(void *cam_param) {
	Ptr<BackgroundSubtractorMOG2> pBackSub;
	pBackSub = createBackgroundSubtractorMOG2(500,8,false);
	int *params = (int*) cam_param;
	VideoCapture cap(0);
	Mat bgframe, frame, prevframe, grayframe, satframe, fgMask, edges, colored;
	Mat frameH, frameS, frameV;
	Mat se1,se2;
	int k, cutx;
	for(int i=0; i<20; i++)
		cap >> frame;
	set_instrument(params[7], params[8], frame);
	namedWindow("Theremultime Demo", WINDOW_FULLSCREEN);

	char H1Tb[50];
	sprintf( H1Tb, "Min Hue");
	createTrackbar(H1Tb,"Theremultime Demo", &H1, 180);
	char H2Tb[50];
	sprintf( H2Tb, "Max Hue");
	createTrackbar(H2Tb,"Theremultime Demo", &H2, 180);
	char OpenTb[50];
	sprintf( OpenTb, "Opening component size");
	createTrackbar(OpenTb,"Theremultime Demo", &openSize, 20, setOpen);
	char CloseTb[50];
	sprintf( CloseTb, "Closing component size");
	createTrackbar(CloseTb,"Theremultime Demo", &closeSize, 20, setClose);

	while( (k = waitKey(20)) != 32){
		if(k == 115){ //if S key pressed
		}
		cap >> frame;
		//Denoise image
		GaussianBlur(frame,frame,Size( 5, 5 ),1.0,0);

		//Apply background subtraction (stored in fgMask)
		pBackSub -> apply(frame,fgMask);
		//imshow("background subtraction",fgMask);
		
		//Increase saturation and detect gloves' blue color
		cvtColor(frame, satframe,COLOR_BGR2HSV);
		vector<Mat> hsvarray(3);
		split(satframe, hsvarray);
		hsvarray[1] *= saturation_rate;
		merge(hsvarray, satframe);
		inRange(satframe, Scalar(H1, S1, V1), Scalar(H2, S2, V2), colored);

		//Multiply color detection and background subtraction
		fgMask = min(fgMask,colored);
		//imshow("before se",fgMask);

		//Denoise with and opening/closing filter
		se1 = getStructuringElement(MORPH_RECT, Size(openSize,openSize));
		se2 = getStructuringElement(MORPH_RECT, Size(closeSize,closeSize));
		morphologyEx(fgMask, fgMask, MORPH_OPEN, se1);
		//imshow("after opening",fgMask);
		morphologyEx(fgMask, fgMask, MORPH_CLOSE, se2);

		//Detect hands' position
		Mat Points, PointsL, PointsR;
		findNonZero(fgMask,Points);
		Rect Min_Rect=boundingRect(Points);
		if (countNonZero(fgMask.col((Min_Rect.tl().x + Min_Rect.br().x)/2))>1){
			cutx = (fgMask.cols + Min_Rect.tl().x + Min_Rect.br().x)/2 % fgMask.cols;
		}else{
			cutx = (Min_Rect.tl().x + Min_Rect.br().x)/2;
		}
		
		//Detect right hand position (image's left side)
		Mat fgMaskL(fgMask, Rect(0,0,cutx,fgMask.rows-1));
		findNonZero(fgMaskL,PointsL);
		Rect Min_Rect_L = boundingRect(PointsL);

		//Detect left hand position (image's right side)
		Mat fgMaskR(fgMask, Rect(cutx,0,fgMask.cols-1-cutx,fgMask.rows-1));
		findNonZero(fgMaskR,PointsR);
		Rect Min_Rect_R = boundingRect(PointsR)+ Point(cutx,0);

		//Draw bounding boxes and distance lines
		rectangle(frame,Min_Rect,Scalar(0,255,0),2);
		rectangle(frame,Min_Rect_L,Scalar(255,0,0),2);
		rectangle(frame,Min_Rect_R,Scalar(0,0,255),2);
		rectangle(fgMask,Min_Rect,Scalar(255,255,255),2);
		//line(fgMask,Point(cutx,0), Point(cutx,fgMask.rows-1), Scalar(255,255,255),2);
		line(frame,Point(Min_Rect_L.tl().x,0), Point(Min_Rect_L.tl().x,frame.rows-1), Scalar(255,255,255),2);
		line(frame,Point(0,Min_Rect_R.br().y), Point(frame.cols,Min_Rect_R.br().y), Scalar(255,255,255),2);

		//Update distance value in params
		params[2] = params[1];
		params[1] = params[0];
		params[0] = Min_Rect_L.tl().x;
		params[5] = params[4];		
		params[4] = params[3];
		params[3] = Min_Rect_R.br().y;
		
		//imshow("satframe", satframe);
		imshow("Theremultime Demo",frame);
		//imshow("fgMask",fgMask);
		//imshow("Theremultime Demo",colored);
	}

	return NULL;
}
