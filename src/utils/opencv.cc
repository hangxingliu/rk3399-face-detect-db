#include "opencv2/opencv.hpp"

#include "./utils.hpp"
#include "../types/base.hpp"
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
		LOG_FATAL_F("copyBGRMat2DataArray: depth is not CV_8U but %d", depth);
		return nullptr;
	}
	if(channels != 3) {
		LOG_FATAL_F("copyBGRMat2DataArray: channels is not 3 but %d", channels);
		return nullptr;
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
		if(!faceHaarCascade.load(haarcascadeXML)) {
			char str[512];
			sprintf(str, "init face haar cascade from %s failed!", haarcascadeXML);
			LOG_FATAL(str);
			return false;
		}
	}
	LOG_DEBUG_F("Initialized face haar cascade from %s", haarcascadeXML);
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

bool detectBlurByLaplacian(
	cv::Mat& image,
	double threshold,
	double* meanValueResult,
	double* varianceResult) {

	cv::Mat lap;
	cv::Mat greyImage;

	if(image.channels() == 1) {
		greyImage = image;
	} else {
		cv::cvtColor(image, greyImage, CV_BGR2GRAY);
	}

	cv::Laplacian(greyImage, lap, CV_64F);
	cv::Scalar _mu, _sigma; // 均值(mean value: 平均值), 标准差(standard deviation: 标准偏差)
	cv::meanStdDev(lap, _mu, _sigma);

	double meanValue = _mu.val[0];
	double standardDeviation = _sigma.val[0];
	double variance = standardDeviation * standardDeviation;

	if(meanValueResult) *meanValueResult = meanValue;
	if(varianceResult) *varianceResult = variance;

	return variance < threshold;
}
bool detectBlurByLaplacian(cv::Mat &image) {
	return detectBlurByLaplacian(image, DEFAULT_LAPLACIAN_BLUR_THRESHOLD, nullptr, nullptr);
}
