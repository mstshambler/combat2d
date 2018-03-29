#include <stdio.h>

#include "Entity.h"

Entity::Entity() {
	pos.Set(0.0f, 0.0f);
	oldPos.Set(0.0f, 0.0f);
	angle = 0.0f;
	speed = 0.0f;
}

Entity::~Entity() {
}

Vector *Entity::GetPos() {
	return &pos;
}

Vector *Entity::GetOldPos() {
	return &oldPos;
}

void Entity::SetAngle(const float &f) {
	angle = f;
}

float Entity::GetAngle() const {
	return angle;
}

void Entity::SetSpeed(const float &f) {
	speed = f;
}

float Entity::GetSpeed() const {
	return speed;
}




AnimatedEntity::AnimatedEntity() {
	animationTime = 0.0f;
}

AnimatedEntity::~AnimatedEntity() {
}

void AnimatedEntity::SetAnimationTime(const float &f) {
	animationTime = f;
}

float AnimatedEntity::GetAnimationTime() const {
	return animationTime;
}
