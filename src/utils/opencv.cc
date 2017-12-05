#include "opencv2/opencv.hpp"

#include "./utils.hpp"
#include "../public_types/base.hpp"
#include "../log/log.hpp"
#include "./opencv.hpp"

int getCVMatDataSize(cv::Mat *mat) { return mat->total() * mat->elemSize(); }

/**
 * @param result allow NULL (NULL means malloc a new space to storage result)
 * @param resultSize return result size
 * @param copyRule
 * @return unsigned char result
 */
unsigned char* copyBGRMat2DataArray(
	cv::Mat &mat,
	unsigned char* result,
	int* resultSize,
	CopyBGRMat2ArrayRule copyRule) {

	int depth = mat.depth();
	int channels = mat.channels();

	if(depth != CV_8U) {
		CREATE_STR_FROM_INT(str, depth, true);
		LOG_FATAL2("copyBGRMat2DataArray: depth is not CV_8U but ", str);
		return DELETE_STR(str), nullptr;
	}
	if(channels != 3) {
		CREATE_STR_FROM_INT(str, channels, true);
		LOG_FATAL2("copyBGRMat2DataArray: channels is not 3 but ", str);
		return DELETE_STR(str), nullptr;
	}

	int rows = mat.rows, cols = mat.cols;
	uchar* p = result;
	if(!result) {
		int size = rows * cols * channels;
		p = (uchar*) malloc(size);
	}
	uchar* ret = p;
	bool toRGB = (copyRule == COPY_RULE_BGR_TO_RGB);

	//optimize for continuous
	if(mat.isContinuous()) { cols *= rows; rows = 1; }

	int i, j, j2 = cols * channels;
	for(i = 0 ; i < rows; i ++ ) {
		uchar* row = mat.ptr<uchar>(i);
		for(j = 0; j < j2 ; j += channels ) {
			*p++ = row[toRGB ? (j+2) : j];
			*p++ = row[j+1];
			*p++ = row[toRGB ? j : (j+2)];
		}
	}
	*resultSize = p - ret;

	return ret;
}



static cv::CascadeClassifier faceHaarCascade;
bool initFaceHaarCascade(const char* haarcascadeXML) {
	if(faceHaarCascade.empty()) {
		return faceHaarCascade.load(haarcascadeXML);
	}
	return true;
}
bool detectFace(
	cv::Mat& image,
	std::vector<cv::Rect> &result,
	bool grayScaleOptimize) {

	if(faceHaarCascade.empty())
		return false;

	#define __I_FACE_DETECT(img) faceHaarCascade.detectMultiScale(img, result, 1.1, 3, 0)
	if(grayScaleOptimize) {
		cv::Mat grayImage;
		cv::cvtColor(image, grayImage, CV_BGR2GRAY);
		__I_FACE_DETECT(grayImage);
	} else {
		__I_FACE_DETECT(image);
	}
	return true;
}
