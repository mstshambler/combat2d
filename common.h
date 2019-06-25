#ifndef _common_h
#define _common_h

#define _USE_MATH_DEFINES
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <string>
#include <list>
#include <map>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;

using namespace std;

template <class T>
class Point {
protected:
	T value[2];

public:
	Point() {
	}

	Point(const T &x, const T &y) {
		Set(x, y);
	}

	~Point() {
	}

	void Set(const T &x, const T &y) {
		value[0] = x;
		value[1] = y;
	}

	void SetX(const T &x) {
		value[0] = x;
	}

	void SetY(const T &y) {
		value[1] = y;
	}

	void Get(T *x, T *y) const {
		if (x)
			*x = value[0];
		if (y)
			*y = value[1];
	}

	T GetX() const {
		return value[0];
	}

	T GetY() const {
		return value[1];
	}
};

class PointInt : public Point<int> {
};

class PointUInt : public Point<uint> {
};

class PointFloat : public Point<int> {
};

#endif