#include "level.h"

LevelTile::LevelTile() {
	floor = 0;
	flags = 0;
}

LevelTile::~LevelTile() {
}

bool LevelTile::IsPassable() {
	if (floor > 0)
		return false;
	return true;
}

void LevelTile::SetFloor(const byte &b) {
	floor = b;
}

byte LevelTile::GetFloor() const {
	return floor;
}

Level::Level(const ushort &x, const ushort &y) {
	size[0] = x;
	size[1] = y;
	tiles = new LevelTile[x * y];
}

Level::~Level() {
	if (tiles)
		delete tiles;
}

LevelTile *Level::GetTile(const ushort &x, const ushort &y) const {
	if (x < 0 || y < 0 || x >= size[0] || y >= size[1])
		return NULL;

	return tiles + x + y * size[0];
}

void Level::GetSize(ushort *x, ushort *y) const {
	if (x)
		*x = size[0];
	if (y)
		*y = size[1];
}

ushort Level::GetSizeX() const {
	return size[0];
}

ushort Level::GetSizeY() const {
	return size[1];
}

bool Level::IsPassable(const ushort &x, const ushort &y) const {
	LevelTile *t;

	t = GetTile(x, y);
	if (t)
		return t->IsPassable();
	else
		return false;
}

Pathfinder::Pathfinder(Level *l) {
	level = l;
	openCells = new uint[l->GetSizeX() * l->GetSizeY()];
	cellsStatus = new byte[l->GetSizeX() * l->GetSizeY()];
	cellsDistance = new float[l->GetSizeX() * l->GetSizeY()];
}

Pathfinder::~Pathfinder() {
	if (openCells)
		delete openCells;
	if (cellsStatus)
		delete cellsStatus;
	if (cellsDistance)
		delete cellsDistance;
}

uint Pathfinder::PackCoords(const ushort &x, const ushort &y) const {
	return x + (y << 16);
}

void Pathfinder::MakeCoords(const uint &value, ushort *x, ushort *y) const {
	if (x)
		*x = value & 0xffff;
	if (y)
		*y = value >> 16;
}

bool Pathfinder::CheckSize(const ushort &x, const ushort &y, const ushort &size) {
	ushort sx;
	ushort sy;

	bool checkSize = true;

	for (sy = 0; sy < size && checkSize; sy++)
			for (sx = 0; sx < size && checkSize; sx++) {
				LevelTile *checkTile;
				
				checkTile = level->GetTile(x + sx, y + sy);
				if (checkTile && checkTile->IsPassable() == 0)
					checkSize = false;
			}

	return checkSize;
}

uint Pathfinder::PrepareNodes(const ushort &startX, const ushort &startY, const ushort &endX, const ushort &endY, const ushort &size) {
	float minDistance;
	uint closestCell;
	float closestCellDistance;

	uint *openCell_cur;
	uint *openCell_max;
	uint *openCell_last;

	{
		uint i;
		uint max = level->GetSizeY() * level->GetSizeX();
		float *clearDist;

		clearDist = cellsDistance;

		for (i = 0; i < max; i++, clearDist++)
			*clearDist = -1.0f;
	}

	memset(openCells, 0, sizeof(uint)* level->GetSizeX() * level->GetSizeY());
	memset(cellsStatus, 0, sizeof(byte)* level->GetSizeX() * level->GetSizeY());

	*openCells = PackCoords(startX, startY);

	closestCell = *openCells;
	closestCellDistance = (float)level->GetSizeX() * level->GetSizeY();
	minDistance = -1.0f;

	openCell_max = openCells + (level->GetSizeX() * level->GetSizeY() - 1);
	openCell_cur = openCells;
	openCell_last = openCell_cur;

	*(cellsDistance + startX + startY * level->GetSizeX()) = 0.0f;

	while(1) {
		ushort x, y;

		uint shift;
		float curDistance;
		float theoryDistance;

		// get current cell
		MakeCoords(*openCell_cur, &x, &y);
		shift = x + y * level->GetSizeX();

		// get current distance 
		curDistance = *(cellsDistance + shift);
		theoryDistance = sqrt(pow((float)(x - endX), 2) + pow(float(y - endY), 2));
		if (theoryDistance < closestCellDistance) {
			closestCellDistance = theoryDistance;
			closestCell = PackCoords(x, y);
		}
		
		// mark current cell as closed
		*(cellsStatus + shift) = 2;
		// if current distance is lower than the best one...
		if (minDistance < 0 || curDistance < minDistance) {
			if (x == endX && y == endY) { // get new best distance
				minDistance = curDistance;
			} else { // move to other tiles
				int tx, ty;

				tx = -1;
				ty = -1;
				while(ty < 2) {
					// if we're not going to move back into our current tile and it's passable, trying to move there
					if ((tx != 0 || ty != 0 ) && x + tx >= 0 && y + ty >= 0 && x + tx < level->GetSizeX() && y + ty < level->GetSizeY()) {
						// check if current size fits
						if (CheckSize(x + tx, y + ty, size) && (tx == 0 || ty == 0 || (CheckSize(x + tx, y, size) && CheckSize(x, y + ty, size)) ) ) {
							uint newShift;
							byte newStatus;
							float newDistance;
							float distanceMod;

							if (tx == 0 || ty == 0)
								distanceMod = 1.0f;
							else
								distanceMod = Math::sqrt2;

							newShift = x + tx + (y + ty) * level->GetSizeX();
							newStatus = *(cellsStatus + newShift);
							newDistance = *(cellsDistance + newShift);

							// if it's a new tile or we've found a better path, get to the tile
							if (newStatus == 0 || newDistance > curDistance + distanceMod) {
								*(cellsDistance + newShift) = curDistance + distanceMod;
								*(cellsStatus + newShift) = 1;
								if (newStatus != 1) {
									if (openCell_last == openCell_max)
										openCell_last = openCells;
									else
										openCell_last++;
									*openCell_last = PackCoords(x + tx, y + ty);
								}
							}
						}
					}
					tx++;
					if (tx == 2) {
						tx = -1;
						ty++;
					}
				}
			}
		}

		if (openCell_last == openCell_cur)
			break;
		if (openCell_cur == openCell_max)
			openCell_cur = openCells;
		else
			openCell_cur++;
	}

	if (minDistance == 0) {
		return closestCell;
	} else {
		return PackCoords(endX, endY);
	}
}

std::list<uint *> *Pathfinder::GetPath(const ushort &startX, const ushort &startY, const ushort &endX, const ushort &endY, const ushort &size, const int &limitDistance) {
	uint closestCell;

	std::list<uint *> *foundPath;

	// Step 1: fill distances
	closestCell = PrepareNodes(startX, startY, endX, endY, size);

	{
		ushort x, y;

		for (y = 0; y < level->GetSizeY(); y++) {
			for (x = 0; x < level->GetSizeX(); x++)
				printf("%5.2f ", *(cellsDistance + x + y * level->GetSizeX()));
			printf("\n");
		}
	}

	// Step 2: get the path
	foundPath = new std::list<uint *>();

	{
		ushort x, y;
		float curDistance;
		uint shift;
		uint *pathTile;

		MakeCoords(closestCell, &x, &y);

		pathTile = new uint(PackCoords(x, y));
		foundPath->push_back(pathTile);

		shift = x + y * level->GetSizeX();
		curDistance = *(cellsDistance + shift);
		while(curDistance > -0.1f) {
			int tx, ty;
			float lowestDistance;
			uint lowestDistanceCell;

			ty = -1;
			tx = -1;

			lowestDistance = curDistance;
			lowestDistanceCell = PackCoords(x, y);

			while (ty < 2) {
				if ((tx != 0 || ty != 0 ) && x + tx >= 0 && y + ty >= 0 && x + tx < level->GetSizeX() && y + ty < level->GetSizeY()) {
					float newDistance;

					newDistance = *(cellsDistance + x + tx + (y + ty) * level->GetSizeX());

					if (newDistance > -0.1f && newDistance < lowestDistance && (tx == 0 || ty == 0 || CheckSize(x + tx, y, size) && CheckSize(x, y + ty, size)) ) {// && newDistance + Math::sqrt2Limit > curDistance) {
						lowestDistance = newDistance;
						lowestDistanceCell = PackCoords(x + tx, y + ty);
					}
				}

				tx++;
				if (tx == 2) {
					tx = -1;
					ty++;
				}

			}
			if (lowestDistanceCell != PackCoords(x, y)) {
				MakeCoords(lowestDistanceCell, &x, &y);

				printf("%i %i\n", x, y);

				pathTile = new uint(lowestDistanceCell);
				foundPath->push_front(pathTile);
				curDistance = lowestDistance;
				if (x == startX && y == startY)
					break;
			} else
				break;
		}
		if (x != startX || y != startY) {
			std::list<uint *>::iterator it;

			for (it = foundPath->begin(); it != foundPath->end(); it++) {
				delete *it;
			}

			foundPath->clear();
		}
	}
	printf("---\n");
	
	// Step 3: optimize the path
	if (foundPath->size() > 0) {
		MathBresenham bresenham;
		std::list<uint *>::iterator lrTile;
		std::list<uint *>::iterator lrLineTile;

		uint *curTile;
		uint *nextTile;
		float curDistance = 0.0f;

		lrTile = foundPath->begin();
		while(lrTile != foundPath->end()) {
			curTile = *lrTile;

			lrLineTile = foundPath->end()--;
			nextTile = *lrLineTile;
			if (nextTile == curTile)
				break;
			while(1) {
				ushort sx, sy, ex, ey;
				int x, y;
				int prevx[2], prevy[2];
				byte ok = 1;
				ushort lastGoodX, lastGoodY;

				MakeCoords(*curTile, &sx, &sy);
				MakeCoords(*nextTile, &ex, &ey);

				lastGoodX = sx;
				lastGoodY = sy;

				prevx[0] = prevx[1] = prevy[0] = prevy[1] = -1;

				Math::BresenhamLineInit(sx, sy, ex, ey, 1, bresenham);
				while(ok == 1) {
					if (Math::BresenhamLineStep(bresenham, &x, &y) == 0)
						ok = 2;
					if (CheckSize((ushort)x, (ushort)y, size) == 0) {
						ok = 0;
						break;
					} else if (prevx[0] != -1 && prevy[0] != -1 && prevx[0] != x && prevy[0] != y && 
						(CheckSize((ushort)prevx[0], (ushort)y, size) == 0 || CheckSize((ushort)x, (ushort)prevy[0], size) == 0)
					) {
						ok = 0;
						break;

					} else if (limitDistance > 0 && curDistance + sqrt(pow((float)(x - sx), 2) + pow((float)(y - sy), 2)) <= limitDistance) {
						lastGoodX = x;
						lastGoodY = y;
					}
					prevx[0] = prevx[1];
					prevy[0] = prevy[1];
					prevx[1] = x;
					prevy[1] = y;
				}
				if (ok) {
					while(1) {
						lrLineTile--;
						nextTile = *lrLineTile;
						if (nextTile == curTile)
							break;
						foundPath->remove(nextTile);
					}
					if (limitDistance > 0) {
						curDistance += sqrt(pow((float)(x - lastGoodX), 2) + pow((float)(y - lastGoodY), 2));
						if (curDistance + 1.0f > limitDistance) {
							while(1) {
								nextTile = foundPath->back();
								if (nextTile == curTile)
									break;
								foundPath->remove(nextTile);
							}
							nextTile = new uint(PackCoords(lastGoodX, lastGoodY));
							foundPath->push_back(nextTile);
						}
					}
					break;
				} else {
					lrLineTile--;
					nextTile = *lrLineTile;
					if (nextTile == curTile)
						break;
				}
			}
			lrTile++;
		}
	}

	return foundPath;
	//return NULL;
}
