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
//	auto font = fontQt("monaspace", 12, Scalar::all(0),
//		CV_FONT_NORMAL, CV_STYLE_NORMAL, 0);
//	addText(image, "Image Location:", Point(0, 0), font);
//	addText(image, FILE, Point(0, 20), font);


	namedWindow(WINDOW);
	createTrackbar("test", WINDOW, NULL, 255, 0 );
//	displayOverlay(WINDOW, FILE, 2000);

	imshow(WINDOW, image);

	waitKey(0);

	return 0;
}
