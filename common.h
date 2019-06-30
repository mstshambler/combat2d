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

class Color {
protected:
	float r, g, b, a;

public:
	Color() {
		r = g = b = a = 1.0f;
	}

	Color(const float &r, const float &g, const float &b, const float &alpha) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = alpha;
	}

	~Color() {}

	void SetR(const float &f) { r = f; }
	void SetG(const float &f) { g = f; }
	void SetB(const float &f) { b = f; }
	void SetAlpha(const float &f) { a = f; }
	float GetR() const { return r; }
	float GetG() const { return g; }
	float GetB() const { return b; }
	float GetAlpha() const { return a; }

	void Set(const float &r, const float &g, const float &b, const float &alpha) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = alpha;
	}

	void Get(float *r, float *g, float *b, float *alpha) const {
		if (r)
			*r = this->r;
		if (g)
			*g = this->g;
		if (b)
			*b = this->b;
		if (alpha)
			*alpha = this->a;
	}
};

#endif