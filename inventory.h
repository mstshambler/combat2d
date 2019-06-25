#ifndef _inventory_h
#define _inventory_h

#include "common.h"

class Item {
protected:
public:
	Item();
	~Item();
};

class Inventory {
protected:
	list<Item *> *items;
public:
	Inventory();
	~Inventory();
};

#endif