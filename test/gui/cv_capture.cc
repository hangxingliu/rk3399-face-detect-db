/// TestEntry: cv_capture
/// Link: ${OpenCV_LIBS}
/// ----------------------

#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;

#define MAX_CAPTURE 5

void listCaptures(int* resultArray, int* length) {
	cv::VideoCapture cap;
	for(int i = 0; i < MAX_CAPTURE ; i ++ ) {
		// if(cap.open())
	}
}

int main() {
	const char* FILE = "./test/images/children-817365_1280.jpg";
	const char* WINDOW = "demo";

	auto image = imread(FILE);
	if(!image.data) {
		printf("Could not load image: %s\n", FILE);
		return 1;
	}

	namedWindow(WINDOW);
	createTrackbar("test", WINDOW, NULL, 255, 0 );

	imshow(WINDOW, image);

	waitKey(0);

	return 0;
}
