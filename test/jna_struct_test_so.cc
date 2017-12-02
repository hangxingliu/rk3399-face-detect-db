/// This test unit is used for testing jna object mapping rules

#include<stdio.h>
#include "./jna_struct_test_so_base.hpp"
#include "./jna_struct_test_so.hpp"

void dumpRect1(Rect1* r) {
	Rect2 r2;
	r2.leftTop = &(r->leftTop);
	r2.rightBottom = &(r->rightBottom);
	dumpRect2(&r2);
}

void dumpRect2(Rect2* r) {
	auto lt = r->leftTop;
	auto rb = r->rightBottom;
	printf("Rect: (%.2f, %.2f)-(%.2f, %.2f)\n", lt->x, lt->y, rb->x, rb->y);
}

int main() {
	float x1 = 11.11f, y1 = 33.33f, x2 = 24.6f, y2 = 10.3f;
	Point p1{x1, y1};
	Point p2{x2, y2};

	Rect1 r1{p1, p2};
	Rect2 r2{&p1, &p2};

	dumpRect1(&r1);
	dumpRect2(&r2);
	return 0;
}
