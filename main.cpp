#include <stdio.h>
#include <stdlib.h>

#include "level.h"

int main() {
	Level *level;
	Pathfinder *pf;
	List<uint> *foundPath;

	level = new Level(10, 5);
	pf = new Pathfinder(level);

	/*
		..........
		......E...
		--------..
		...S......
		..........
	*/
	level->GetTile(0, 2)->SetFloor(1);
	level->GetTile(1, 2)->SetFloor(1);
	level->GetTile(2, 2)->SetFloor(1);
	level->GetTile(3, 2)->SetFloor(1);
	level->GetTile(4, 2)->SetFloor(1);
	level->GetTile(5, 2)->SetFloor(1);
	level->GetTile(6, 2)->SetFloor(1);
	level->GetTile(7, 2)->SetFloor(1);

	foundPath = pf->GetPath(7, 3, 6, 1, 1, -1);

	if (foundPath && foundPath->GetCount() > 0) {
		uint *tile;
		ListReader<uint> *lr;

		lr = new ListReader<uint>();
		lr->Attach(foundPath);

		tile = lr->GoFirstRef();
		while(tile) {
			ushort x, y;

			x = *tile & 0xffff;
			y = *tile >> 16;
			printf("%i %i\n", x, y);
			tile = lr->GoNextRef();
		}
	} else {
		printf("No path\n");
	}
	{
		char buf[256];
		gets(buf);
	}
	return 0;
}