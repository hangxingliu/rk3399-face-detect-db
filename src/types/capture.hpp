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

typedef struct Capture_FrameRequestOpts {
	/// 1: convert BGR to RGB (slowly)
	/// @todo add this function if it is necessary
	int convert2RGB = 0;
} Capture_FrameRequestOpts;

typedef struct Capture_FrameImageInfo {
	/// 0: Unique Id of frame image
	int frameId = 0;

	/// 0: BGR(in default)  1: RGB
	int isRGB = 0;
	/// image data size
	int size = 0;
	/// width of image, height of image
	int w = 0, h = 0;
} Capture_FrameImageInfo;




#endif // CAPTURE_HPP
