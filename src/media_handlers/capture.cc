#include<stdlib.h>
#include<dirent.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include <linux/videodev2.h>

#include "opencv2/opencv.hpp"

#include "../log/log.hpp"
#include "../types/base.hpp"
#include "../types/capture.hpp"

/// Default capture device id. 0 => /dev/video0
const int defaultCapture = 0;
#define MAX_CAPTURES 10

#define IS_NUMBER_CHAR(x)  (x >= '0' && x <= '9')

bool isCaptureInitialized = false;

bool thisVariableUseForUnitTest = false;
void Capture_enableUnitTest() { thisVariableUseForUnitTest = true; }

const char* SYSTEM_DEVICE_PATH = "/dev";
const char* CAPTURE_DEVICE_PREFIX = "video";

int GlobalCaptureDeviceLen = 0;
PhysicalCaptureDevice GlobalCaptureDevices[MAX_CAPTURES];
PhysicalCaptureDevice *GlobalCaptureDev;

cv::VideoCapture *GlobalCapture;

// =============================
//    Methods about V4L2
static void copyStrInV4L2Capability(char* to, __u8* from) {
	char* p1 = to; __u8* p2 = from; __u8 v;
	while(*p2) {
		v = *p2++;
		if(v > 127) v = ' ';
		*p1++ = (char) v;
	}
	*p1='\0';
}
void physicalCaptureDeviceToString(char* result, PhysicalCaptureDevice* dev, bool markAsUse) {
	if(dev->available)
		sprintf(result, "  capture %d: %s (%s %s) %s",
			dev->id, dev->devicePath, dev->cardName, dev->driver,
			(markAsUse ? " <<< (Use)" : " "));
	else
		sprintf(result, "  capture %d: %s NOT-AVAILABLE", dev->id, dev->devicePath);
}
static bool isDeviceACapture(const char* deviceName) {
	const char* p1 = CAPTURE_DEVICE_PREFIX;
	const char* p2 = deviceName;
	while(*p1)
		if( !p2 || (*p1++ != *p2++) )
			return false;
	return true;
}
static int getDeviceId(const char* deviceName) {
	int len = strlen(deviceName);

	/// return in function getDeviceId and free resource
	#define GET_DEVICE_ID_RETURN(v) DELETE_STR(nameWithQuote), v;
	// /dev/video0 => "/dev/video0"
	CREATE_STR(nameWithQuote, (len+4)); addQuote2String(deviceName, nameWithQuote);

	if(len < 2) {
		LOG_FATAL2("getDeviceId: Invalid device name (it is too short); name: ", nameWithQuote);
		return GET_DEVICE_ID_RETURN(-1);
	};

	char c1 = deviceName[len-1], c2 = deviceName[len-2];
	if(!IS_NUMBER_CHAR(c1)) {
		LOG_FATAL2("getDeviceId: Invalid device name (not a number); name: ", nameWithQuote);
		return GET_DEVICE_ID_RETURN(-1);
	}
	if(IS_NUMBER_CHAR(c2)) {
		LOG_WARN2("getDeviceId: Id of device >= 10; name: ", nameWithQuote);
		return GET_DEVICE_ID_RETURN(-1);
	}

	return c1 - '0';
}

// =============================
//    Methods about OpenCV

static int initGlobalCapture() {
	if(!GlobalCaptureDev)
		return LOG_FATAL("Please invoke Capture_Init before invoking initGlobalCapture()"), -1;
	if(GlobalCapture)
		delete GlobalCapture;
	GlobalCapture = new cv::VideoCapture(GlobalCaptureDev->id);
	if(!GlobalCapture->isOpened())
		return LOG_FATAL("Open capture failed!"), -1;

	return 0;
}

int Capture_Init() {
	if(isCaptureInitialized)
		return 0;

	auto dir = opendir(SYSTEM_DEVICE_PATH);
	if(!dir) {
		LOG_FATAL("Capture_Init: could not open system device directory!");
		return -1;
	}

	GlobalCaptureDeviceLen=0;
	int availableDeviceLen=0;

	struct dirent* entry;
	struct v4l2_capability videoCap;
	while((entry = readdir(dir))) {
		auto deviceName = entry->d_name;
		if(!isDeviceACapture(deviceName))
			continue;

		int id = getDeviceId(deviceName);
		auto dev = &GlobalCaptureDevices[GlobalCaptureDeviceLen++];
		dev->id = id;
		sprintf(dev->devicePath, "%s/%s", SYSTEM_DEVICE_PATH, deviceName);

		int fd;
		if((fd = open(dev->devicePath, O_RDWR)) < 0) {
			dev->available = false;
			char warn[2048];
			sprintf(warn, "Capture_Init: open %s failed(%d): %s",
				dev->devicePath, errno, strerror(errno));
			LOG_WARN(warn);
			continue;
		}

		if(ioctl(fd, VIDIOC_QUERYCAP, &videoCap) < 0) {
			dev->available = false;
			char warn[2048];
			sprintf(warn, "Capture_Init: ioctl %s with VIDIOC_QUERYCAP failed(%d): %s",
				dev->devicePath, errno, strerror(errno));
			LOG_WARN(warn);
		} else {
			dev->available = true;
			availableDeviceLen++;
			copyStrInV4L2Capability(dev->cardName, videoCap.card);
			copyStrInV4L2Capability(dev->driver, videoCap.driver);
		}
		close(fd);
	}
	closedir(dir);

	if(availableDeviceLen == 0)
		return LOG_FATAL("Capture_Init: There is no capture available!"), -1;

	//Choose capture
	GlobalCaptureDev = NULL;
	if(defaultCapture < GlobalCaptureDeviceLen &&
		GlobalCaptureDevices[defaultCapture].available) {
		GlobalCaptureDev = &GlobalCaptureDevices[defaultCapture];
	} else {
		LOOP_TIMES(i, GlobalCaptureDeviceLen) {
			auto dev = &GlobalCaptureDevices[i];
			if(dev->available) {
				GlobalCaptureDev = dev;
				break;
			}
		}
	}

	//Dump
	CREATE_STR(strDump, 2048);
	LOG_INFO("Capture list:");
	LOOP_TIMES(i, GlobalCaptureDeviceLen) {
		auto dev = &GlobalCaptureDevices[i];
		physicalCaptureDeviceToString(strDump, dev, dev->id == GlobalCaptureDev->id);
		LOG_INFO(strDump);
	}
	DELETE_STR(strDump);

	//Open capture
	if(initGlobalCapture() < 0)
		return -1;

	// Take a frame into image for unit test
	if(thisVariableUseForUnitTest) {
		cv::Mat frame;
		(*GlobalCapture) >> frame;
		cv::imwrite("unit_test_capture_frame.jpg", frame);
	}

	isCaptureInitialized = true;
	return 0;
}

int Capture_Read(cv::Mat *frame) {
	if(!GlobalCaptureDev)
		return LOG_FATAL("Please invoke Capture_Init before invoking Capture_Read()"), -1;
	return GlobalCapture->read(*frame);
}
