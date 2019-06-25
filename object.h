#ifndef _object_h
#define _object_h

#include "common.h"
#include "inventory.h"

class ObjectPattern {
protected:
	byte size[2];
	byte *pattern;

public:
	ObjectPattern();
	ObjectPattern(const byte &x, const byte &y);
	~ObjectPattern();

	void SetSize(const byte &x, const byte &y);
	void GetSize(byte *x, byte *y) const;

	void SetPattern(const byte &x, const byte &y, const byte &val);
	byte GetPattern(const byte &x, const byte &y) const;

};

class Object {
protected:
	wstring title;
	ushort pos[2];
	byte size[2];
	ObjectPattern *pattern;
	int hp[2];
	byte armor;
	byte cover;
	byte angle;

	Inventory *inventory;

private:
	void ModifySize();

public:
	Object();
	~Object();

	void SetTitle(const wstring &s);
	wstring *GetTitle();

	void SetPos(const ushort &x, const ushort &y);
	void GetPos(ushort *x, ushort *y) const;

	void SetArmor(const byte &b);
	byte GetArmor() const;

	void SetCover(const byte &b);
	byte GetCover() const;

	void SetAngle(const byte &b);
	byte GetAngle() const;

	Inventory *GetInventory();
	
	void SetPattern(ObjectPattern *obj);
	ObjectPattern *GetPattern();

	byte GetPatternCell(const byte &x, const byte &y) const;

	void GetSize(byte *x, byte *y) const;
};

#endif