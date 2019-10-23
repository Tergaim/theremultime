#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;
using namespace cv;

int main() {
	VideoCapture cap(0);
	while(waitKey(20) <= 0){
		Mat I;
		cap >> I;
		imshow("I",I);
	}	
	return 0;
}
