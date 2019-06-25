#include "gui.h"
#include "game.h"

#include "ui_funcs.h"

using namespace std;

extern Game *game;

byte GameExitButtonClick(void *element, const int &x, const int &y) {
	game->SetExitGame(1);
	return 1;
}