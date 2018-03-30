#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <list>

#include <time.h>
#ifndef WIN32
#include <sys/time.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common.h"
#include "gettimeofday.h"
#include "render.h"
#include "texture.h"
#include "gui.h"
#include "math.h"
#include "list.h"

#include "level.h"

/*
int main() {
	std::list<uint *> *uList;
	uint *ui;
	uint *u1, *u2, *u3, *u4, *u5;

	uList = new std::list<uint *>();

	ui = new uint(0);
	u1 = ui;
	uList->push_back(ui);
	ui = new uint(1);
	u2 = ui;
	uList->push_back(ui);
	ui = new uint(2);
	u3 = ui;
	uList->push_back(ui);
	ui = new uint(3);
	u4 = ui;
	uList->push_back(ui);
	ui = new uint(4);
	u5 = ui;
	uList->push_back(ui);
	
	for (std::list<uint *>::iterator it = uList->begin(); it != uList->end(); it++) {
		ui = *it;
	}

	uList->clear();

	delete uList;
	
	delete u1;
	delete u2;
	delete u3;
	delete u4;
	delete u5;
	{
		char buf[256];
		gets(buf);
	}
	return 0;
}
*/
/*
int main() {
	Level *level;
	Pathfinder *pf;
	List<uint> *foundPath;

	level = new Level(10, 5);
	pf = new Pathfinder(level);

	level->GetTile(0, 2)->SetFloor(1);
	level->GetTile(1, 2)->SetFloor(1);
	level->GetTile(2, 2)->SetFloor(1);
	level->GetTile(3, 2)->SetFloor(1);
	level->GetTile(4, 2)->SetFloor(1);
	level->GetTile(5, 2)->SetFloor(1);
	level->GetTile(6, 2)->SetFloor(1);
	level->GetTile(7, 2)->SetFloor(1);

	foundPath = pf->GetPath(6, 1, 3, 3, 1, -1);

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
*/

// keyboard
bool global_keys[GLFW_KEY_LAST];
bool global_keysOnce[GLFW_KEY_LAST];

// mouse
int global_mouse[GLFW_MOUSE_BUTTON_LAST];
float global_mousepos[2];

Render *renderer;
Texture *texturer;
GUI *gui;

struct timeval tick_otime, tick_ntime, tick_diff;

static void CalculateFrame(void) {
	float overall_msecs;
	float local_mousepos[2];

	gettimeofday(&tick_ntime, NULL);
	getdifftime(tick_ntime, tick_otime, &tick_diff);
	tick_otime = tick_ntime;

	overall_msecs = tick_diff.tv_sec * 1000 + tick_diff.tv_usec / 1000.0f;
	// slow time!
	if (overall_msecs > 20.0f)
		overall_msecs = 20.0f;

	renderer->ConvertWindowToScreenCoords(global_mousepos[0], global_mousepos[1], local_mousepos[0], local_mousepos[1]);

	// Do something!

	if (global_mouse[0] == 2)
		global_mouse[0] = 0;
	if (global_mouse[1] == 2)
		global_mouse[1] = 0;

	memset(global_keysOnce, 0, sizeof(bool) * GLFW_KEY_LAST);
}

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key != GLFW_KEY_UNKNOWN) {
		if (action == GLFW_PRESS) {
			global_keys[key] = true;
			global_keysOnce[key] = true;
		} else if (action == GLFW_RELEASE) {
			global_keys[key] = false;
		}
	}

	printf("Key: %i %i Status: %i Mods: %i\n", key, scancode, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double x, double y) {
	global_mousepos[0] = (float)x;
	global_mousepos[1] = (float)y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	global_mouse[button] = action == GLFW_PRESS ? true : false;

	printf("Mouse: %i %i %i\n", button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	printf("Scroll: %i %i\n", xoffset, yoffset);
}

int main(void)
{
	srand(time(NULL));

	GLFWwindow* window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    window = glfwCreateWindow(1024, 768, "Combat2D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW");
		exit(EXIT_FAILURE);
	}

	renderer = new Render();
	renderer->Init(1024, 768);

	texturer = new Texture();
	texturer->LoadTexturesList();

	gui = new GUI(texturer, renderer);

	{
		int i;

		// clear
		for (i = 0; i < GLFW_KEY_LAST; i++)
			global_keys[i] = false;
		for (i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
			global_mouse[i] = false;
	}
	
	while ( !glfwWindowShouldClose(window) ) {
		glfwPollEvents();

		CalculateFrame();

		renderer->Begin();
		renderer->SetTextureMode();
		renderer->Set2DMode();

		gui->DrawString(0.0f, 768.0f, "Ninja", 16.0f, 2.0f);
		gui->DrawCursor(global_mousepos[0], global_mousepos[1], 0);

		renderer->End2DMode();
		renderer->End();

		glfwSwapBuffers(window);       
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
