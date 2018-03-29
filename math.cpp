#include <stdio.h>
#include <math.h>

#include "math.h"

MathBresenham::MathBresenham() {
	x = 0;
	y = 0;
	endX = 0;
	endY = 0;
	diffX = 0;
	diffY = 0;
	error = 0;
	base = 0;
	dirX = 0;
	dirY = 0;
	halfStep = 0;
	useHalfStep = 0;
}

MathBresenham::~MathBresenham() {
}




float Math::sqrt2 = sqrt(2.0);
float Math::sqrt2Limit = sqrt(2.0) + 0.01;

int Math::Max(const int &v1, const int &v2) {
	if (v1 > v2)
		return v1;
	else
		return v2;
}

void Math::BresenhamLineInit(const int &sx, const int &sy, const int &ex, const int &ey, const int &use_halfstep, MathBresenham &bresenham) {
	bresenham.diffX = (ex > sx ? (bresenham.dirX = 1, ex - sx) : (bresenham.dirX = -1, sx - ex)) << 1;
	bresenham.diffY = (ey > sy ? (bresenham.dirY = 1, ey - sy) : (bresenham.dirY = -1, sy - ey)) << 1;

	if (bresenham.diffY > bresenham.diffX) {
		int t;

		t = bresenham.diffX;
		bresenham.diffX = bresenham.diffY;
		bresenham.diffY = t;

		t = bresenham.dirX;
		bresenham.dirX = bresenham.dirY;
		bresenham.dirY = t;

		bresenham.x = sy;
		bresenham.y = sx;

		bresenham.endX = ey;
		bresenham.endY = ex;

		bresenham.base = 1;
	} else {
		bresenham.x = sx;
		bresenham.y = sy;

		bresenham.endX = ex;
		bresenham.endY = ey;

		bresenham.base = 0;
	}
	bresenham.halfStep = 0;
	bresenham.useHalfStep = use_halfstep;
	bresenham.error = bresenham.diffY - (bresenham.diffX >> 1);
}

int Math::BresenhamLineStep(MathBresenham &bresenham, int *x, int *y) {
	if (bresenham.base == 0) {
		if (x) *x = bresenham.x;
		if (y) *y = bresenham.y;
	} else {
		if (x) *x = bresenham.y;
		if (y) *y = bresenham.x;
	}

	if (bresenham.x == bresenham.endX && bresenham.halfStep == 0) {
		return 0;
	}

	if (bresenham.halfStep) {
		bresenham.y += bresenham.dirY;
		bresenham.error -= bresenham.diffX;
		bresenham.halfStep = 0;
	}
	else {
		if ((bresenham.error == 0 && bresenham.diffX > 0) || bresenham.error > 0) {
			if (bresenham.useHalfStep)
				bresenham.halfStep = 1;
			else {
				bresenham.y += bresenham.dirY;
				bresenham.error -= bresenham.diffX;
			}
		}
		bresenham.x += bresenham.dirX;
		bresenham.error += bresenham.diffY;
	}
	return 1;
}
