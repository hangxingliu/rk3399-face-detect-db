/// TestEntry: blur_detect_time_consuming
/// Description: test blur detection time consumming
/// Link: rk3399_face_detect_db_debug
/// Link: ${OpenCV_LIBS}
/// With: ../../src/utils/opencv.cc
/// With: ../../src/log/methods.cc
/// ----------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <chrono>

#include "opencv2/opencv.hpp"

#include "../../utilities/index.hpp"
#include "../../../src/api/api.hpp"
#include "../../../src/utils/opencv.hpp"

using namespace std::chrono;

Capture_FrameImageInfo frame;

const int EXIT_COUNT = 50;
int count = 0;

high_resolution_clock::time_point _timer_begin, _timer_end;
double _timer_elapsed = 0;
#define _timer_now(var) var = high_resolution_clock::now();
#define timer_start() _timer_now(_timer_begin);
#define timer_end(forWhat) { _timer_now(_timer_end);\
	_timer_elapsed = duration_cast<milliseconds>(_timer_end - _timer_begin).count();\
	printf("\x1b[90m%.0lf ms elapsed in %s\x1b[0m\n", _timer_elapsed, forWhat); }

class BlurInfo {
	double variance;
	bool _isBlurry;
	char toStringBuffer[128];
public:
	BlurInfo() : BlurInfo(0.0, false) {}
	BlurInfo(double variance, bool isBlurry) {
		this->variance = variance;
		this->_isBlurry = isBlurry;
	}
	char* toString() {
		snprintf(toStringBuffer, sizeof(toStringBuffer),
			"%.2lf%s", variance, (_isBlurry ? "(Blurry)" : " ") );
		return toStringBuffer;
	}
	bool isBlurry() { return _isBlurry; }
};

class Statistics {
	bool init = false;
public:
	double min, max, average;
	void add(double num) {
		if(!init) {
			min = max = average = num;
			init = true;
		} else {
			if(num < min) min = num;
			if(num > max) max = num;
			average = (average + num ) * 0.5f;
		}
	}
};

int main() {
	printf("[.] Testing blur detection time consumming ...\n");

	GlobalConfig config("./test-env/arm-only/workspace");
	face_set_init_flag(2); // no database
	face_set_debug_log(0, 0); // no log
	int status = face_init(&config);
	if(status != 0) {
		testFailedFormat("face_init return %d", status);
		return -1;
	}
	testPassed("face_init");

	cv::Mat images[EXIT_COUNT];
	BlurInfo blurInfo[EXIT_COUNT];
	Capture_FrameImageInfo info;
	Statistics stat;

	char desc[128];
	for(count = 0 ; count < EXIT_COUNT; count ++ ) {
		snprintf(desc, 128, "capture frame %d", count);
		timer_start();
		if(face_get_frame(&info) != 0) {
			testFailed("get capture frame");
			break;
		}
		timer_end(desc);
		cv::Mat image(info.h, info.w, CV_8UC3, *info.data);
		images[count] = image.clone();

		snprintf(desc, 128, "detect blur for frame %d", count);
		timer_start();
		double meanValue = 0.0;
		double variance = 0.0;
		bool isBlurry = detectBlurByLaplacian(image, DEFAULT_LAPLACIAN_BLUR_THRESHOLD,
			&meanValue, &variance);
		blurInfo[count] = BlurInfo(variance, isBlurry);
		stat.add(variance);
		timer_end(desc);
		printf("blurry: %.4lf\n", variance);

		usleep(30 * 1000); //30ms
	}

	printf("\nBlurry Statistics:\n  min: %.2f\n  ave: %.2f\n  max: %.2f\n",
		stat.min, stat.average, stat.max);
	printf("Colloect frame images done. prepare for display images ...\n");


	const char* WIN_NAME = "blurry test";
	cv::namedWindow(WIN_NAME);

	auto colorOK = CV_RGB(154,205,50);
	auto colorBad = CV_RGB(139,0,0);
	auto colorInfo = CV_RGB(30,144,255);
	for(count = 0 ; count < EXIT_COUNT; count ++ ) {
		snprintf(desc, sizeof(desc), "Index: %d (%p)", count, &images[count]);

		auto info = &(blurInfo[count]);
		auto image = images[count];
		cv::putText(image, info->toString(), cv::Point(5, 35),
			cv::FONT_HERSHEY_DUPLEX, 1.0f, info->isBlurry() ? colorBad : colorOK );

		cv::putText(image, desc, cv::Point(image.cols - 235, 25),
			cv::FONT_HERSHEY_DUPLEX, 0.5f, colorInfo);

		cv::imshow(WIN_NAME, image);

		cv::waitKey(0);
	}

	return 0;
}
