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

int GameExitButtonClick(void *element, const int &x, const int &y) {
	GUIElementButton *e = (GUIElementButton *)element;

	if (e->GetParent()) {
		GUIElementMultilineText *mt;
		mt = (GUIElementMultilineText *)e->GetParent()->FindElement(L"mainMenuMultilineText");

		if (mt) {
			mt->SetScroll(mt->GetScroll() + 10.0f);
			if (mt->GetScroll() > mt->GetMaxScroll())
				mt->SetScroll(mt->GetMaxScroll());
		}
	}

	printf("Exit click: %i %i\n", x, y);
	wprintf(L"Elem: %s %s\n", e->GetId()->c_str(), e->GetText()->c_str());
	return 1;
}