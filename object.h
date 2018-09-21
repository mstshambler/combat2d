#ifndef _object_h
#define _object_h

#include <list>
#include <string>

#include "level.h"
#include "inventory.h"

using namespace std;

class ObjectPattern {
protected:
	byte size[2];
	byte *pattern;

public:
	ObjectPattern();
	~ObjectPattern();
};

class Object {
protected:
	string title;
	int pos[2];
	ObjectPattern *pattern;
	int hp[2];
	byte material;
	byte armor;
	byte cover;
	byte isPlaceable;
	byte isPlacableSurface;
	byte angle;

	Inventory *inventory;

public:
	Object();
	~Object();
};

#endif