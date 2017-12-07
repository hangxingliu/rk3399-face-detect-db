/// TestEntry: utils_opencv
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

	const char* FILE = "./test/images/sudo-rm-rf-meme.png";
	const char* WINDOW = "test";

	Mat rawImage = imread(FILE);
	Mat rawImageWithAlpha = imread(FILE, cv::IMREAD_UNCHANGED);
	int channels = rawImage.channels();
	int cols = rawImage.cols, rows = rawImage.rows;
	int expectLength = 0, expectLength2 = 0;

	printf("Image cols: %d; rows: %d; channels: %d\n", cols, rows, channels);
	printf("Image cols: %d; rows: %d; channels: %d ((UNCHANGED))\n",
		rawImageWithAlpha.cols, rawImageWithAlpha.rows, rawImageWithAlpha.channels());

	if(channels != 3) {
		sprintf(str, "rawImage.channels() != 3, but %d", channels);
		return testFailed(str);
	}

	uchar* dataBGR = nullptr;
	int dataBGRLen = 0;
	expectLength = rawImage.cols * rawImage.rows * channels;
	expectLength2 = rawImage.elemSize() * rawImage.total();

	if(expectLength != expectLength2) {
		sprintf(str, "expectLength { %d } != expectLength2 { %d }",
			expectLength, expectLength2);
		return testFailed(str);
	}

	dataBGR = copyBGRMat2DataArray(rawImage, dataBGR, &dataBGRLen, COPY_RULE_KEEP_BGR);

	if(dataBGRLen != expectLength) {
		sprintf(str, "COPY_RULE_KEEP_BGR: resultLength { %d } != expectLength { %d }",
			dataBGRLen, expectLength);
		return testFailed(str);
	}
	sprintf(str, "copied! result length: %d", dataBGRLen);
	testPassed(str);

	uchar* dataRGB = (uchar*) malloc(expectLength);
	int dataRGBLen = 0;
	expectLength = rawImage.cols * rawImage.rows * channels;
	uchar* result = copyBGRMat2DataArray(rawImage, dataRGB, &dataRGBLen, COPY_RULE_BGR_TO_RGB);

	if(result != dataRGB)
		return testFailed("copyBGRMat2DataArray malloc new memory even though provide memory space parameter!");

	if(dataRGBLen != expectLength) {
		sprintf(str, "COPY_RULE_BGR_TO_RGB: resultLength { %d } != expectLength { %d }",
			dataRGBLen, expectLength);
		return testFailed(str);
	}

	uchar* left = dataBGR; uchar* right = dataRGB;
	for(int i = 0; i < expectLength ; i += 3) {
		if(left[i] != right[i+2] || left[i+1] != right[i+1] || left[i+2] != right[i]) {
			return testFailed("copy failed! One of RGB and BGR is wrong");
		}
	}
	testPassed("Checked RGB and BGR!");

	auto imageRGB = Mat(rows, cols, CV_8UC3, dataRGB);
	auto imageBGR = Mat(rows, cols, CV_8UC3, dataBGR);
	namedWindow(WINDOW);

	printf("showing RGB image ...");
	imshow(WINDOW, imageRGB);
	waitKey();

	printf("showing BGR image ...");
	imshow(WINDOW, imageBGR);
	waitKey();

	return testDone("Utils OpenCV module test");
}
