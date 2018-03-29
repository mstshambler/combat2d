#ifndef _entity_h
#define _entity_h

#include "math.h"

class Entity {
protected:
	Vector pos;
	Vector oldPos;
	float angle;
	float speed;

public:
	Entity();
	~Entity();

	Vector *GetPos();
	Vector *GetOldPos();

	void SetAngle(const float &f);
	float GetAngle() const;

	void SetSpeed(const float &f);
	float GetSpeed() const;
};

class AnimatedEntity : public Entity {
protected:
	float animationTime;

public:
	AnimatedEntity();
	~AnimatedEntity();

	void SetAnimationTime(const float &f);
	float GetAnimationTime() const;
};

#endif