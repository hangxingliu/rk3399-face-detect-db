/// TestEntry: call_debug_so_get_frame
/// Link: rk3399_face_detect_db_debug
/// Link: ${OpenCV_LIBS}
/// ----------------------

#include<stdio.h>
#include<string.h>

#include "../utilities/index.hpp"
#include "../../src/api/api.hpp"
#include "../../src/types/capture.hpp"

#define SIZE 1024
char str[SIZE];

using namespace cv;

#define INIT_FLAG_NO_CAPTURE 1
#define INIT_FLAG_NO_DATABASE 2

int main() {
	const char* WINDOW = "Frame in capture";

	face_set_init_flag(INIT_FLAG_NO_DATABASE);
	if( face_init(NULL) != 0 )
		return testFailed("face_init");

	Capture_FrameImageInfo info;

	if( face_get_frame(&info) != 0)
		return testFailed("face_get_frame");

	cv::Mat image(info.h, info.w, CV_8UC3, *(info.data) );

	namedWindow(WINDOW);
	imshow(WINDOW, image);
	waitKey();

	return testDone("Get Frame by calling API from debug so file");
}
