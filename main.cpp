#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;
using namespace cv;

int main() {
	VideoCapture cap(0);
	VideoCapture cap2(2);
	while(waitKey(20) <= 0){
		Mat I, J;
		cap >> I;
		cap2 >> J;
		imshow("I",I);
		imshow("J",J);
	}
	return 0;
}
