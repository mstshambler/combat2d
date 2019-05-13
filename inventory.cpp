#include "inventory.h"

Inventory::Inventory() {
	items = new list<Item *>;
}

Inventory::~Inventory() {
	if (items)
		delete items;
}