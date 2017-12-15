/// TestEntry: cv_capture_queue
/// Link: ${OpenCV_LIBS}
/// Link: ${CMAKE_THREAD_LIBS_INIT}
///
/// This test is used to prove that opencv VideoCapture get frame is not synchronized
/// ----------------------

#include <stdio.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

const char* WIN = "Capture display";

Mat frame;
mutex grabbing;
atomic<bool> grab;

VideoCapture capture(0);

void ThreadGrab() {
	grab.store(true);

	while(grab.load() == true) {
		grabbing.lock();
		capture >> frame;
		grabbing.unlock();

		this_thread::sleep_for(chrono::milliseconds(10));
	}
}
void ThreadEcho() {
	grab.store(true);
	while(grab.load() == true) {
		printf("hello\n");
		this_thread::sleep_for(chrono::milliseconds(500));
		fflush(stdout);
	}
}


int main_correct() {
	thread t(ThreadGrab);
	t.detach();

	namedWindow(WIN);
	int count = 0;

	count = 10;
	while(true) {
		grabbing.lock();
		bool empty = frame.empty();
		grabbing.unlock();
		if(!empty) break;
		this_thread::yield();
	}
	while(count--) {
		grabbing.lock();
		printf("frame id: %d\n", count);
		imshow(WIN, frame);
		grabbing.unlock();
		waitKey(0);
	}
	grab.store(false);

	return 0;
}
int main_incorrect() {
	thread t(ThreadEcho);
	t.detach();

	namedWindow(WIN);
	int count = 0;

	count = 10;
	while(count--) {
		capture >> frame;
		imshow(WIN, frame);

		waitKey(0);
	}

	return 0;
}

int main() {
//	return main_incorrect();
	return main_correct();
}
