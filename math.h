#ifndef _math_h
#define _math_h


#include "common.h"

class MathBresenham {
public:
	int x;
	int y;
	int endX;
	int endY;
	int diffX;
	int diffY;
	int error;
	byte base;
	int dirX;
	int dirY;
	byte halfStep;
	byte useHalfStep;

	MathBresenham();
	~MathBresenham();
};

class Math {
public:
	static int Max(const int &v1, const int &v2);

	static void BresenhamLineInit(const int &startX, const int &startY, const int &ex, const int &ey, const int &use_halfstep, MathBresenham &bresenham);
	static int BresenhamLineStep(MathBresenham &bresenham, int *x, int *y);

	static float sqrt2;
	static float sqrt2Limit;
};
#endif
