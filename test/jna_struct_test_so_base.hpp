#ifndef JNA_STRUCT_TEST_SO_BASE_HPP
#define JNA_STRUCT_TEST_SO_BASE_HPP

typedef struct Point {
	float x,y;
} Point;

typedef struct Rect1 {
	Point leftTop;
	Point rightBottom;
} Rect1;

typedef struct Rect2 {
	Point* leftTop;
	Point* rightBottom;
} Rect2;

#endif // JNA_STRUCT_TEST_SO_BASE_HPP
