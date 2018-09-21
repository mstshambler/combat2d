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

class Vector {
protected:
	float value[2];

public:
	Vector();
	Vector(const float &x, const float &y);
	~Vector();

	void Set(const Vector &v);
	void Set(const float &x, const float &y);
	void SetX(const float &x);
	void SetY(const float &y);

	void Get(Vector &res) const;
	void Get(float &x, float &y) const;
	float GetX() const;
	float GetY() const;

	void Add(const Vector &v);
	void Add(const Vector &v1, const Vector &v2);
	void Add(const float &x, const float &y);

	void Sub(const Vector &v);
	void Sub(const Vector &v1, const Vector &v2);
	void Sub(const float &x, const float &y);

	void Scale(const float &f);
	void Scale(const float &f, Vector &res) const;
	
	float DotProduct(const Vector &v) const;
	float DotProduct(const float &x, const float &y) const;
	
	float Length() const;

	void Normalize();
	void Normalize(Vector &res) const;

	void Inverse();
	void Inverse(Vector &res) const;
};

class Math {
public:
	static int Max(const int &v1, const int &v2);

	static void BresenhamLineInit(const int &startX, const int &startY, const int &ex, const int &ey, const int &use_halfstep, MathBresenham &bresenham);
	static int BresenhamLineStep(MathBresenham &bresenham, int *x, int *y);

	static float sqrt2;

	static float DegreeToRadian(const float &f);
	static float RadianToDegree(const float &f);

	static void CalculateVector(const float &f, Vector &res);
	static float CalculateAngle(const Vector &from, const Vector &to);

	static float CalculateRayCircleIntersectionDistance(const Vector &ray_start, const Vector &ray_move_norm, const Vector &circle_center, const float &circle_radius);
};
#endif
