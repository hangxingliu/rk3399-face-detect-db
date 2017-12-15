#include "opencv2/opencv.hpp"

#include "./global.hpp"
#include "../log/log.hpp"
#include "../types/base.hpp"
#include "../types/api_error_no.hpp"

static cv::Mat staticFrameBuffer[FRAME_BUFFER_COUNT];
static bool staticIsFrameBufferValid[FRAME_BUFFER_COUNT];

static int staticFrameBufferSwitcher = 0;

int FrameBuffer_init() {
	LOOP_TIMES(i, FRAME_BUFFER_COUNT) {
		staticIsFrameBufferValid[i] = false;
	}
	return 0;
}

bool FrameBuffer_isBufferValid(int bufferId) {
	if(bufferId < 0 && bufferId >= FRAME_BUFFER_COUNT)
		return false;
	return staticIsFrameBufferValid[bufferId];
}
void FrameBuffer_setValid(int bufferId) { staticIsFrameBufferValid[bufferId] = true; }
void FrameBuffer_setInvalid(int bufferId) { staticIsFrameBufferValid[bufferId] = false; }

cv::Mat* FrameBuffer_giveMeBuffer(int* bufferId) {
	lockFrameSwitcher();

	int v = staticFrameBufferSwitcher;

	staticFrameBufferSwitcher++;
	if(staticFrameBufferSwitcher >= FRAME_BUFFER_COUNT)
		staticFrameBufferSwitcher = 0;

	unlockFrameSwitcher();

	if(v >= 0 && v < FRAME_BUFFER_COUNT) {
		*bufferId = v;
		return &(staticFrameBuffer[v]);
	}

	return NULL;
}

int FrameBuffer_cloneBuffer(int bufferId, cv::OutputArray cloneTo) {
	lockFrameAccess(bufferId);
	if(!FrameBuffer_isBufferValid(bufferId)) {
		unlockFrameAccess(bufferId);
		return API_FRAME_ID_IS_INVALID;
	}

	staticFrameBuffer[bufferId].copyTo(cloneTo);
	unlockFrameAccess(bufferId);
	return 0;
}

