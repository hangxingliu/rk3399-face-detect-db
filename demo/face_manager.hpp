#ifndef FACE_MANAGER_HPP
#define FACE_MANAGER_HPP

class FaceManager {
public:
	cv::Scalar color[8];
	int count = 0;
	cv::Rect rect[128];

	FaceManager() {
		color[0] = CV_RGB(116, 198, 53);
		color[1] = CV_RGB(135, 223, 57);
		color[2] = CV_RGB(255, 166, 58);
		color[3] = CV_RGB(255, 197, 62);
		color[4] = CV_RGB(216, 48, 58);
		color[5] = CV_RGB(235, 54, 65);
		color[6] = CV_RGB(0, 176, 242);
		color[7] = CV_RGB(0, 212, 241);
	}
	void clear() {count = 0;}
	void add(int x0, int y0, int x1, int y1) {
		rect[count++] = cv::Rect(cv::Point(x0, y0), cv::Point(x1, y1));
	}
	void drawToImage(cv::Mat &image) {
		for(int i = 0 ; i < count; i ++ )
			cv::rectangle(image, rect[i], color[i%8], 3);
	}
};


#endif // FACE_MANAGER_HPP
