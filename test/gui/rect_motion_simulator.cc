/// TestEntry: rect_motion_simulator
/// Link: ${OpenCV_LIBS}
/// ----------------------


#include<stdio.h>
#include<string.h>
#include<chrono>
#include "../utilities/index.hpp"
#include "../../src/utils/rect_motion_simulator.hpp"
#include "opencv2/opencv.hpp"

#define MAX_W 640
#define MAX_H 320
#define MAX_RECT_SIZE 200
#define MIN_RECT_SIZE 50

using namespace cv;
using namespace std::chrono;

int continuousEnableCount = 1;

class MyRect {
public:
	bool enable; double x, y, w, h;
	MyRect(): MyRect(false, 0,0,0,0) {}
	MyRect(bool enable, double x, double y, double w, double h) {
		this->enable = enable; this->x = x; this->y = y; this->w = w; this->h = h;
	}
	MyRect clone() { return MyRect(enable, x, y, w, h); }
	Rect getCVRect() { return Rect((int)x,(int)y,(int)w,(int)h); }
	static MyRect random() {
		bool enable = continuousEnableCount > 0 ? (rand() % 4 != 1) : (rand() % 4 == 1);
		continuousEnableCount = enable ? (continuousEnableCount+1) : 0;
		if(!enable)
			return MyRect(false, 0,0,0,0);
		int w = (rand() % (MAX_RECT_SIZE - MIN_RECT_SIZE)) + MIN_RECT_SIZE;
		int h = (rand() % (MAX_RECT_SIZE - MIN_RECT_SIZE)) + MIN_RECT_SIZE;
		int x = rand() % (MAX_W - w);
		int y = rand() % (MAX_H - h);
		return MyRect(true, x, y, w, h);
	}
};

const char* WIN_NAME = "rect motion simulator";

MyRect r0, r1;
RectMotionSimulator motion;
Mat image(MAX_H, MAX_W, CV_8UC3, Scalar(255, 255, 255));

char str[1024];
int i = 0;

cv::Scalar colorGrey = CV_RGB(176,196,222);
cv::Scalar colorCurrent = CV_RGB(50,205,50);
cv::Scalar colorInfo = CV_RGB(65,105,225);


double now() {
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	return (double) ms.count();
}

double startTime = now();
void loop(int i) { //30ms: 33fps
	if(i % 30 == 0) { // 1 fps
		r0 = r1.clone();
		r1 = MyRect::random();
		motion.addStatus(r1.enable, now(), r1.x, r1.y, r1.w, r1.h);
	}

	Mat imageShow = image.clone();
	if(r0.enable)
		rectangle(imageShow, r0.getCVRect(), colorGrey, 1);
	if(r1.enable)
		rectangle(imageShow, r1.getCVRect(), colorGrey, 1);

	auto current = motion.getStatus(now());
	rectangle(imageShow, Rect(
		(int)current.x, (int)current.y,
		(int)current.w, (int)current.h
		), colorCurrent, 2);

	sprintf(str, "%.1lf s (Press any key to stop)", (now() - startTime) * 0.001 );
	cv::putText(imageShow, str, Point(50, 30), cv::FONT_HERSHEY_DUPLEX, 0.5f, colorInfo);

	imshow(WIN_NAME, imageShow);
}

int main() {
	namedWindow(WIN_NAME);

	printf("Press any key to stop\n");

	startTime = now();
	int i = 0;
	while(++i) {
		loop(i);
		int key = waitKey(30) & 0xff;
		if(key != 0xff)
			break;
	}

	return testDone("rect_motion_simulator test");
}
