#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h> 

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"
#include "game.h"

#include "ui_funcs.h"

using namespace std;

extern Game *game;

byte GameExitButtonClick(void *element, const int &x, const int &y) {
	game->SetExitGame(1);
	return 1;
}