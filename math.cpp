#define _USE_MATH_DEFINES
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

Vector::Vector() {
	value[0] = value[1] = 0.0f;
}

Vector::Vector(const float &x, const float &y) {
	value[0] = x;
	value[1] = y;
};

Vector::~Vector() {
}

void Vector::Set(const Vector &v) {
	 value[0] = v.GetX();
	 value[1] = v.GetY();
}

void Vector::Set(const float &x, const float &y) {
	value[0] = x;
	value[1] = y;
}

void Vector::SetX(const float &x) {
	value[0] = x;
}

void Vector::SetY(const float &y) {
	value[1] = y;
}

void Vector::Get(Vector &res) const {
	res.Set(value[0], value[1]);
}

void Vector::Get(float &x, float &y) const {
	x = value[0];
	y = value[1];
}

float Vector::GetX() const {
	return value[0];
}

float Vector::GetY() const {
	return value[1];
}

void Vector::Add(const Vector &v) {
	value[0] += v.GetX();
	value[1] += v.GetY();
}

void Vector::Add(const Vector &v1, const Vector &v2) {
	value[0] = v1.GetX() + v2.GetX();
	value[1] = v1.GetY() + v2.GetY();
}

void Vector::Add(const float &x, const float &y) {
	value[0] += x;
	value[1] += y;
}

void Vector::Sub(const Vector &v) {
	value[0] -= v.GetX();
	value[1] -= v.GetY();
}

void Vector::Sub(const Vector &v1, const Vector &v2) {
	value[0] = v1.GetX() - v2.GetX();
	value[1] = v1.GetY() - v2.GetY();
}

void Vector::Sub(const float &x, const float &y) {
	value[0] -= x;
	value[1] -= y;
}

void Vector::Scale(const float &f) {
	value[0] *= f;
	value[1] *= f;
}

void Vector::Scale(const float &f, Vector &res) const {
	res.Set(value[0] * f, value[1] * f);
}
	
float Vector::DotProduct(const Vector &v) const {
	return value[0] * v.GetX() + value[1] * v.GetY();
}

float Vector::DotProduct(const float &x, const float &y) const {
	return value[0] * x + value[1] * y;
}
	
float Vector::Length() const {
	return sqrt(value[0] * value[0] + value[1] * value[1]);
}

void Vector::Normalize() {
	float l;

	l = Length();
	if (l < 0.0001)
		l = 1.0f;

	value[0] /= l;
	value[1] /= l;
}

void Vector::Normalize(Vector &res) const {
	float l;

	l = Length();
	if (l < 0.0001)
		l = 1.0f;

	res.Set(value[0] / l, value[1] / l);
}

void Vector::Inverse() {
	value[0] = -value[0];
	value[1] = -value[1];
}

void Vector::Inverse(Vector &res) const {
	res.Set(-value[0], -value[1]);
}

float Math::sqrt2 = sqrt(2.0f);

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

float Math::DegreeToRadian(const float &f) {
	return f / 180.0f * M_PI;
}

float Math::RadianToDegree(const float &f) {
	return f / M_PI * 180.0f;
}

void Math::CalculateVector(const float &f, Vector &res) {
	res.Set(cos(DegreeToRadian(f)), sin(DegreeToRadian(f)));
}

float Math::CalculateAngle(const Vector &from, const Vector &to) {
	Vector diff;
	Vector norm;
	float angle;

	diff.Sub(to, from);
	diff.Normalize();

	norm.Set(1.0f, 0.0);

	angle = diff.DotProduct(norm);
	angle = RadianToDegree(acos(angle));

	if (to.GetY() < from.GetY())
		angle = 360.0f - angle;

	return angle;
}

float Math::CalculateRayCircleIntersectionDistance(const Vector &ray_start, const Vector &ray_move_norm, const Vector &circle_center, const float &circle_radius) {
	Vector diff;
	float rs_len;
	float rp_len;
	float d;

	diff.Sub(circle_center, ray_start);
	rs_len = diff.Length();

	rp_len = diff.DotProduct(ray_move_norm);

	if (fabs(rp_len) < 0.0001f)
		return -1.0f;

	d = pow(circle_radius, 2) - (pow(rs_len, 2) - pow(rp_len, 2));
	if (d < 0.0f)
		return -1.0f;

	return rp_len - sqrt(d);
}
