#ifndef _math_h
#define _math_h

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
	static float DegreeToRadian(const float &f);
	static float RadianToDegree(const float &f);

	static void CalculateVector(const float &f, Vector &res);
	static float CalculateAngle(const Vector &from, const Vector &to);

	static float CalculateRayCircleIntersectionDistance(const Vector &ray_start, const Vector &ray_move_norm, const Vector &circle_center, const float &circle_radius);
};

#endif