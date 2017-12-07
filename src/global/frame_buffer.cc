#include "opencv2/opencv.hpp"

#include "./global.hpp"
#include "../types/base.hpp"
#include "../log/log.hpp"

static cv::Mat staticFrameBuffer[FRAME_BUFFER_COUNT];
static bool staticFrameUsed[FRAME_BUFFER_COUNT];

static int staticFrameBufferSwitcher = 0;

int FrameBuffer_init() {
	LOOP_TIMES(i, FRAME_BUFFER_COUNT) {
		staticFrameUsed[i] = false;
	}
	return 0;
}

bool FrameBuffer_exist(int id) {
	if(id >= 0 && id < FRAME_BUFFER_COUNT)
		return false;

	return staticFrameUsed[id];
}

cv::Mat* FrameBuffer_giveMeBuffer(int* accessLockId) {
	int v = -1;
	lockFrameSwitcher();

	v = staticFrameBufferSwitcher;

	staticFrameBufferSwitcher++;
	if(staticFrameBufferSwitcher >= FRAME_BUFFER_COUNT)
		staticFrameBufferSwitcher = 0;

	unlockFrameSwitcher();

	if(v >= 0 && v < FRAME_BUFFER_COUNT) {
		*accessLockId = v;
		return &(staticFrameBuffer[v]);
	}

	return NULL;
}


