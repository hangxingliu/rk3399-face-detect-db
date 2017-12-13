/// TestEntry: cv_img_window
/// Link: ${OpenCV_LIBS}
/// ----------------------

#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

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
