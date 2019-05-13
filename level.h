#ifndef _level_h
#define _level_h

#include <list>

#include "common.h"
#include "math.h"

#include "object.h"

using namespace std;

class LevelTile {
protected:
	byte floor;
	byte flags;

	Object *object;

public:
	LevelTile();
	~LevelTile();

	bool IsPassable();
	
	void SetFloor(const byte &f);
	byte GetFloor() const;

	void SetObject(Object *obj);
	Object *GetObject();
};

class Level {
protected:
	ushort size[2];
	LevelTile *tiles;

public:
	Level(const ushort &x, const ushort &y);
	~Level();

	LevelTile *GetTile(const ushort &x, const ushort &y) const;
	void GetSize(ushort *x, ushort *y) const;
	ushort GetSizeX() const;
	ushort GetSizeY() const;
	bool IsPassable(const ushort &x, const ushort &y) const;
	void PlaceObject(Object *obj, const ushort &x, const ushort &y);
	void RemoveObject(Object *obj);
};

class Pathfinder {
protected:
	Level *level;
	uint *openCells;
	byte *cellsStatus;
	int *cellsDistance;

	enum CellStatus {
		CellStatus_Unknown = 0,
		CellStatus_Open,
		CellStatus_Closed
	};

public:
	Pathfinder(Level *l);
	~Pathfinder();

	list<uint *> *GetPath(const ushort &startX, const ushort &startY, const ushort &endX, const ushort &endY, const ushort &size = 1, const int &limitDistance = -1);
	uint PrepareNodes(const ushort &startX, const ushort &startY, const ushort &endX, const ushort &endY, const ushort &size = 1);
	bool CheckSize(const ushort &x, const ushort &y, const ushort &size = 1);

	uint PackCoords(const ushort &x, const ushort &y) const;
	void MakeCoords(const uint &value, ushort *x, ushort *y) const;
};

#endif