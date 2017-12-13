/// TestEntry: call_api_on_arm
/// Link: rk3399_face_detect_db_debug
/// OnlyForARM: true
/// ----------------------
#include<stdio.h>
#include<unistd.h>

#include "../../utilities/index.hpp"
#include "../../../src/api/api.hpp"
#include "../../../src/types/capture.hpp"
#include "../../../src/types/config.hpp"

int main() {
	printf("[.] Testing debug API on ARM64 device ...");
	int status = 0;

	GlobalConfig config("./test-env/arm-only/workspace");
	status = face_init(&config);
	if(status != 0) {
		testFailedFormat("face_init return %d", status);
		return -1;
	}
	testPassed("face_init");

	Capture_FrameImageInfo frame;

	for(int i = 0 ; i < 10 ; i ++ ) {
		status = face_get_frame(&frame);
		if(status != 0) {
			testFailedFormat("face_get_frame return %d", status);
			return -1;
		}
		testPassedFormat("face_get_frame {width: %d, height: %d, id: %d}",
			frame.w, frame.h, frame.frameId);

		usleep(100 * 1000); // 100ms
	}

	return 0;
}
