#include "opencv2/opencv.hpp"

#include "./global.hpp"
#include "../log/log.hpp"
#include "../types/base.hpp"
#include "../types/api_error_no.hpp"
#include "../types/capture.hpp"

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

	LOG_FATAL_F("Could not choose a buffer space. Terrible exception! (v = %d)", v);
	return NULL;
}

int FrameBuffer_getLastFrameId() {
	lockFrameSwitcher();
	int frameIndex = staticFrameBufferSwitcher;
	unlockFrameSwitcher();

	return (frameIndex <= 0 ? (FRAME_BUFFER_COUNT) : frameIndex) - 1;
}

//int FrameBuffer_cloneLastBuffer(cv::OutputArray cloneTo) {
//	lockFrameSwitcher();
//	int frameIndex = staticFrameBufferSwitcher - 1;
//	if(frameIndex < 0) frameIndex = FRAME_BUFFER_COUNT - 1;
//	unlockFrameSwitcher();

//	if(!FrameBuffer_isBufferValid(frameIndex)) { return API_FRAME_ID_IS_INVALID; }

//	lockFrameAccess(frameIndex);
//	staticFrameBuffer[frameIndex].copyTo(cloneTo);
//	unlockFrameAccess(frameIndex);

//	return 0;
//}


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

int FrameBuffer_getFrameInfo(bool addLocker, int bufferId, Capture_FrameImageInfo* result) {
	if(addLocker) lockFrameAccess(bufferId);
	if(!FrameBuffer_isBufferValid(bufferId)) {
		if(addLocker) unlockFrameAccess(bufferId);
		return API_FRAME_ID_IS_INVALID;
	}

	auto buffer = &(staticFrameBuffer[bufferId]);
	int width = buffer->cols;
	int height = buffer->rows;

	result->w = width;
	result->h = height;
	result->frameId = bufferId;
	result->size = width * height * 3;
	result->data = &(buffer->data);

	int status = 0;
	// =========================
	// Validate image of frame
	if(!buffer->isContinuous())
		status = (API_FRAME_IS_NOT_CONTINUOUS);
	if(width <= 0 || height <= 0 || !buffer->data)
		status = (API_FRAME_IS_EMPTY);
#ifdef DEBUG
	if(buffer->channels() != 3 || buffer->elemSize() != 3)
		status = (API_FRAME_IS_NOT_U8C3);
#endif
	// =========================

	if(addLocker) unlockFrameAccess(bufferId);
	return status;
}

