#include "object.h"

ObjectPattern::ObjectPattern() {
	size[0] = size[1] = 1;
	pattern = new byte[size[0] * size[1]];
}

ObjectPattern::ObjectPattern(const byte &x, const byte &y) {
	size[0] = x;
	size[1] = y;
	pattern = new byte[x * y];
}

ObjectPattern::~ObjectPattern() {
	if (pattern)
		delete pattern;
}

void ObjectPattern::SetSize(const byte &x, const byte &y) {
	size[0] = x;
	size[1] = y;
	if (pattern)
		delete pattern;
	pattern = new byte[x * y];
}

void ObjectPattern::GetSize(byte *x, byte *y) const {
	if (x)
		*x = size[0];
	if (y)
		*y = size[1];
}

void ObjectPattern::SetPattern(const byte &x, const byte &y, const byte &val) {
	if (x < size[0] && y < size[1])
		*(pattern + x + y * size[0]) = val;
}

byte ObjectPattern::GetPattern(const byte &x, const byte &y) const {
	if (x < size[0] && y < size[1])
		return *(pattern + x + y * size[0]);
	return 0;
}



Object::Object() {
	title = L"";
	pos[0] = pos[1] = 0;
	size[0] = size[1] = 0;
	pattern = NULL;
	hp[0] = hp[1] = 0;
	armor = 0;
	cover = 0;
	angle = 0;

	inventory = new Inventory();
}

Object::~Object() {
	if (inventory)
		delete inventory;
}

void Object::SetTitle(const wstring &s) {
	title = s;
}

wstring *Object::GetTitle() {
	return &title;
}

void Object::SetPos(const ushort &x, const ushort &y) {
	pos[0] = x;
	pos[1] = y;
}

void Object::GetPos(ushort *x, ushort *y) const {
	if (x)
		*x = pos[0];
	if (y)
		*y = pos[1];
}

void Object::SetArmor(const byte &b) {
	armor = b;
}

byte Object::GetArmor() const {
	return armor;
}

void Object::SetCover(const byte &b) {
	cover = b;
}

byte Object::GetCover() const {
	return cover;
}

void Object::SetAngle(const byte &b) {
	angle = b;
	ModifySize();
}

byte Object::GetAngle() const {
	return angle;
}

Inventory *Object::GetInventory() {
	return inventory;
}

void Object::SetPattern(ObjectPattern *obj) {
	pattern = obj;
	ModifySize();
}

ObjectPattern *Object::GetPattern() {
	return pattern;
}

byte Object::GetPatternCell(const byte &x, const byte &y) const {
	byte ox, oy;
	if (!pattern || x >= size[0] || y >= size[1] || x < 0 || y < 0)
		return 0;
	switch (angle) {
		case 0:
			ox = x;
			oy = y;
			break;
		case 1:
			ox = y;
			oy = size[0] - x - 1;
			break;
		case 2:
			ox = size[0] - x - 1;
			oy = size[1] - y - 1;
			break;
		case 3:
			ox = size[1] - y - 1;
			oy = x;
			break;
	}
	return pattern->GetPattern(ox, oy);
}

void Object::ModifySize() {
	byte x, y;

	if (!pattern)
		return;

	pattern->GetSize(&x, &y);

	if (angle == 0 || angle == 2) {
		size[0] = x;
		size[1] = y;
	} else {
		size[0] = y;
		size[1] = x;
	}
}

void Object::GetSize(byte *x, byte *y) const {
	if (x)
		*x = size[0];
	if (y)
		*y = size[1];
}
