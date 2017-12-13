#ifndef FACE_FACE_ADDON_HPP
#define FACE_FACE_ADDON_HPP

class MaxRect {
	int maxArea = 0;
	int maxId = -1;
public:
	void addRect(int id, FF_Rect* rect) {
		int area = (rect->right - rect->left ) * (rect->bottom - rect->top);
		if(area < maxArea)
			return;
		maxArea = area;
		maxId = id;
	}
	int getMaxId() { return maxId; }
};


#endif // FACE_FACE_ADDON_HPP
