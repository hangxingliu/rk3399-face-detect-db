/// TestEntry: utils_opencv_face
/// With: ../src/utils/convert2string.cc
/// With: ../src/utils/opencv.cc
/// With: ../src/log/methods.cc
/// Link: ${OpenCV_LIBS}
/// ----------------------

#include<stdio.h>
#include<string.h>

#include "opencv2/opencv.hpp"
#include "../utilities/index.hpp"
#include "../../src/types/base.hpp"
#include "../../src/utils/utils.hpp"

#define SIZE 1024
char str[SIZE];

using namespace cv;

int main() {

	const char* XML = "./resources/haarcascade_frontalface_alt2.xml";
	const char* IMAGE = "./test/images/children-817365_1280.jpg";
	const char* WINDOW = "OpenCV Face detection";

	auto image = imread(IMAGE);
	if(!image.data) {
		sprintf(str, "load image `%s`", IMAGE);
		return testFailed(str);
	}
	testPassed("loaded image");

	if(!initFaceHaarCascade(XML)) {
		sprintf(str, "init cascade `%s`", XML);
		return testFailed(str);
	}
	testPassed("initialized face haar cascade classifier");

	std::vector<Rect> result;
	detectFace(image, result, false);

	int length = result.size();
	int expectLength = 2;
	if(length!=expectLength) {
		sprintf(str, "face count { %d } != expect count { %d }", length, expectLength);
		testFailed(str);
	} else {
		sprintf(str, "detected %d faces in image.", length);
		testPassed(str);
	}

	Scalar color = CV_RGB(255, 0, 0);
	for (int i = 0; i < length;i++) {
		Rect rect = result[i];
		rectangle(image, rect, color, 2);
	}

	namedWindow(WINDOW);
	imshow(WINDOW, image);
	waitKey();

	return testDone("Utils OpenCV face detection test");
}
