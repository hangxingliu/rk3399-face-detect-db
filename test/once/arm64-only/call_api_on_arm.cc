/// TestEntry: call_api_on_arm
/// Link: rk3399_face_detect_db_debug
/// Link: ${OpenCV_LIBS}
/// Link: ${CMAKE_THREAD_LIBS_INIT}
/// OnlyForARM: true
/// ----------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <thread>
#include <atomic>
#include <chrono>

#include "opencv2/opencv.hpp"

#include "../../utilities/index.hpp"
#include "../../../src/api/api.hpp"
#include "../../../src/types/capture.hpp"
#include "../../../src/types/config.hpp"

using namespace std::chrono;

Capture_FrameImageInfo frame;

const int PAUSE_COUNT = 50;
int count = 0;

std::atomic<int> lastFrameId(-1);
std::atomic<bool> stop(false);

Detect_FaceInfoArray faces = nullptr;

high_resolution_clock::time_point _timer_begin, _timer_end;
double _timer_elapsed = 0;
#define _timer_now(var) var = high_resolution_clock::now();
#define timer_start() _timer_now(_timer_begin);
#define timer_end() { _timer_now(_timer_end);\
	_timer_elapsed = duration_cast<milliseconds>(_timer_end - _timer_begin).count();\
	printf("\x1b[90m%.0lf ms elapsed\x1b[0m\n", _timer_elapsed); }

char command[256];
char* readCommand(const char* prompt) {
	printf("\x1b[34m%s", prompt);
	command[0] = 0;
	scanf("%s", command);
	printf("\x1b[0m");//reset color
	return command;
}
bool strEquals(const char* a, const char* b) {
	return strcmp(a, b) == 0;
}
bool yesOrNo(const char* prompt) {
	auto cmd = readCommand(prompt);
	return strEquals(cmd, "yes");
}

void ThreadGetFrame() {
	printf("thread get frame started!\n");fflush(stdout);

	stop.store(false);
	while(stop.load() != true) {
		int status = face_get_frame(&frame);
		if(status != 0) {
			testFailedFormat("face_get_frame return %d", status);
			break;
		}
		lastFrameId.store(frame.frameId);

//		testPassedFormat("face_get_frame {width: %d, height: %d, id: %d}",
//			frame.w, frame.h, frame.frameId);
//		cv::Mat frameImage(frame.h, frame.w, CV_8UC3, *(frame.data));
//		cv::imwrite("frame.jpg", frameImage);

		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}

	stop.store(true);
	printf("thread get frame finished!\n");fflush(stdout);
}

bool loop() {
	if(count++ >= PAUSE_COUNT) {
		if(!yesOrNo("Paused, continue? (yes/no) > "))
			return false;
		count = 0;
	}
	int frame = lastFrameId.load();
	if(frame < 0)
		return true;

	int faceCount = 0;

	timer_start();
	int status = face_detect(frame, 1, &faceCount, &faces);
	if(status != 0) {
		testFailedFormat("face_detect return %d", status);
		return false;
	}
	timer_end();

	if(faceCount > 0) {
		printf("Found %d face from frame %d!\n", faceCount, frame);
		auto face = faces[0];

		ucharArray preview = nullptr; int w, h;
		Detect_FaceRectAttr attr = {0, 255, 0, 3};
		status = face_draw_face_rect(frame, &attr, &face, &preview, &w, &h);
		if(status != 0 || !preview) {
			testFailedFormat("Could not generate face matched rectangle image! (%d)", status);
			return false;
		}
		cv::Mat image(h,w,CV_8UC3, preview);
		if(!cv::imwrite("face.jpg", image))
			return testFailed("Could not save face.jpg"), false;
		free(preview);

		if(face.matched) {
			printf(">>> matched user in database: \"%s\"(score: %.4f)\n",
				face.matchedUserId, face.matchedScore);
			auto del = readCommand("Do you want to delete it from database? (delete/no) > ");
			if(strEquals(del, "delete")) {
				DB_Modification modi;
				modi.type = DB_MODIFICATION_DELETE;
				modi.userId = face.matchedUserId;
				if(face_update(&modi) == 0) {
					testPassed("face_update");
				} else {
					testFailed("face_update");
					return false;
				}
			} else {
				return true;
			}
		} else {
			if(yesOrNo("Do you want to save this face in database? (yes/no) > ")) {
				auto name = readCommand("input username > ");
				DB_Modification modi;
				modi.type = DB_MODIFICATION_UPDATE;
				modi.userId = name;
				modi.featureLength = face.featureLength;
				modi.features = face.features;
				if(face_update(&modi) == 0) {
					testPassed("face_update");
				} else {
					testFailed("face_update");
					return false;
				}
			} else {
				return true;
			}
		}

		auto cmd = readCommand("continue? (yes/no) > ");
		if(!strEquals(cmd, "yes"))
			return false;

		return true;
	}

	printf("No face in frame %d!\n", frame);
	return true;
}

int main() {
	printf("[.] Testing debug API on ARM64 device ...\n");

	GlobalConfig config("./test-env/arm-only/workspace");
	face_set_debug_log(0, 0); // hide get frame information
	int status = face_init(&config);
	if(status != 0) {
		testFailedFormat("face_init return %d", status);
		return -1;
	}
	testPassed("face_init");

	std::thread getFrame(ThreadGetFrame);
	getFrame.detach();

	while(loop()) {
		if(faces)
			free(faces);
		faces = nullptr;

//		printf("sleeping ... \n");
		usleep(100 * 1000); //100ms
		if(stop.load() == true)
			break;
	}
	stop.store(true);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));//waiting for get frame thread close

	return 0;
}
