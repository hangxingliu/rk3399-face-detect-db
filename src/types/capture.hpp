#include "./base.hpp"

#ifndef PUBLIC_TYPES_CAPTURE_HPP
#define PUBLIC_TYPES_CAPTURE_HPP

typedef struct PhysicalCaptureDevice {
	bool available;
	int id;
	char devicePath[32];
	char cardName[32];
	char driver[16];
} PhysicalCaptureDevice;


typedef struct Capture_FrameImageInfo {
	/// 0: Unique Id of frame image
	int frameId = 0;

	/// image data size
	int size = 0;
	/// width of image
	int w = 0;
	/// height of image
	int h = 0;

	/// @todo
	/// point to data of image
	ucharArray* data = nullptr;
} Capture_FrameImageInfo;




#endif // CAPTURE_HPP
