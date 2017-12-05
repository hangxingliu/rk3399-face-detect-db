/// This test unit is used for testing jna object mapping rules

#include<stdio.h>
#include<stdlib.h>
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

void getRectPathWay1(Path* pathInitedPoints) {
	int xy[] = { 0,0, 0,10, 10,10, 10,0, -1,-1 };
	Path* p = pathInitedPoints;
	p->count = 4;
	for(int i = 0; ;i++) {
		int x=xy[i<<1], y = xy[(i<<1)|1];
		if(x==-1&&y==-1) break;
		p->points[i].x = x;
		p->points[i].y = y;
	}
}

void getRectPathWay2(Path* pathWithoutInit) {
	int xy[] = { 0,0, 0,10, 10,10, 10,0, -1,-1 };
	Path* p = pathWithoutInit;
	p->points = (Point*) malloc(sizeof(Point) * 4);
	p->count = 4;
	for(int i = 0; ;i++) {
		int x=xy[i<<1], y = xy[(i<<1)|1];
		if(x==-1&&y==-1) break;
		p->points[i].x = x;
		p->points[i].y = y;
	}
}
void disposePointsInPath(Path* path) {
	if(path->points)
		free(path->points);
}

int main() {
	float x1 = 11.11f, y1 = 33.33f, x2 = 24.6f, y2 = 10.3f;
	Point p1{x1, y1};
	Point p2{x2, y2};

	Rect1 r1{p1, p2};
	Rect2 r2{&p1, &p2};

	dumpRect1(&r1);
	dumpRect2(&r2);

	Path path;
	Point points[20];
	path.points = points;
	getRectPathWay1(&path);
	printf("path.points.length = %d, p0=(%.2f,%.2f), p1=(%.2f,%.2f) ...\n",
		path.count, path.points[0].x, path.points[0].y, path.points[1].x, path.points[1].y);

	Path path2;
	getRectPathWay2(&path2);
	printf("path2.points.length = %d, p0=(%.2f,%.2f), p1=(%.2f,%.2f) ...\n",
		path2.count, path2.points[0].x, path2.points[0].y, path2.points[1].x, path2.points[1].y);
	disposePointsInPath(&path2);
	return 0;
}
