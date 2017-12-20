#include <stdio.h>

#include "./api.hpp"

#include "../global/global.hpp"
#include "../media/media.hpp"
#include "../face/face.hpp"
#include "../storage/storage.hpp"

#define INIT_FLAG_NO_CAPTURE 1
#define INIT_FLAG_NO_DATABASE 2
static int initFlagForTest = 0;

static void dumpCompileDate() {
#ifdef COMPILE_DATE
	LOG_INFO_F("shared library compiled date: %s", COMPILE_DATE);
#endif
}


static int init(GlobalConfig* config) {
	int status = 0;

	dumpCompileDate();

	// Initialize capture
	if(initFlagForTest & INIT_FLAG_NO_CAPTURE) {
		LOG_INFO("Skip initialization for capture (INIT_FLAG_NO_CAPTURE)");
	} else {
		status = Capture_Init();
		if(status != 0) return API_CANNOT_INIT_CAPTURE;
	}

	status = FrameBuffer_init();
	if(status != 0) return API_CANNOT_INIT_BUFFER;

	status = Config_initGlobalConfig(config);
	if(status != 0) return status;

	if(initFlagForTest & INIT_FLAG_NO_DATABASE) {
		LOG_INFO("Skip initialization for database (INIT_FLAG_NO_DATABASE)");
	} else {
		status = DB_init();
		if(status != 0) return status;
	}

#ifndef FOR_ARM
	initFaceHaarCascade("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
#else
	// For ARM release:
	status = Face_init();
	if(status != 0) return status;
#endif
	return 0;
}


// ==========================================
//    E x p o r t   F u n c t i o n s
void face_set_init_flag(int flag) {
	LOG_API_INVOKE("set_init_flag", "%d", flag);
	initFlagForTest = flag;
}

static const char* API_NAME = "init";
int face_init(GlobalConfig* config) {
	LOG_API_INVOKE(API_NAME, "%p", config);
	APITimer timer(API_NAME);

	int status = init(config);

	timer.click();
	return status;
}
