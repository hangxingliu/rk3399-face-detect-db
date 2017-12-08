/// TestEntry: cpp_pointer_as_parameter
/// ----------------------

#include<stdio.h>
#include<strings.h>
#include<stdlib.h>
#include<memory.h>
#include<time.h>

#include "../../utilities/index.hpp"

typedef struct Point {
	int x = 0; int y = 0;
} Point;

void getAnyPoints(int count, Point* result) {
	for(int i = 0 ; i < count ; i ++ ) {
		result->x = 10;
		result->y = 20;
		result++;
	}
}

int main() {
	Point p[10];
	getAnyPoints(5, p);
	for(int i = 0 ; i < 10 ; i ++ )
		printf("Point[%d] = {x: %d, y: %d}\n", i, p[i].x, p[i].y);
	return 0;
}
