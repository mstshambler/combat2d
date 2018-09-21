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

using namespace std;
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

struct timeval tick_otime, tick_ntime, tick_diff, tick_gameStart;

int clamp_mode;

float cameraPos[2];

byte pattern[10][10] = {
	{ 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
};

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key != GLFW_KEY_UNKNOWN) {
		if (action == GLFW_PRESS) {
			global_keys[key] = true;
			// global_keysOnce[key] = true;

			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, 1);

		} else if (action == GLFW_RELEASE) {
			global_keys[key] = false;
		}
	}

//	printf("Key: %i %i Status: %i Mods: %i\n", key, scancode, action, mods);
}

static void cursor_position_callback(GLFWwindow* window, double x, double y) {
	global_mousepos[0] = (float)x;
	global_mousepos[1] = (float)y;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	global_mouse[button] = action == GLFW_PRESS ? true : false;

//	printf("Mouse: %i %i %i\n", button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//	printf("Scroll: %i %i\n", xoffset, yoffset);
}

void doTick() {
	float overall_msecs;

	gettimeofday(&tick_ntime, NULL);
	getdifftime(tick_ntime, tick_otime, &tick_diff);
	overall_msecs = tick_diff.tv_sec * 1000 + tick_diff.tv_usec / 1000.0f;
	if (overall_msecs > 20.0f)
		overall_msecs = 20.0f;

	if (global_keys[GLFW_KEY_A])
		cameraPos[0] += overall_msecs / 1000.0f;
	else if (global_keys[GLFW_KEY_D])
		cameraPos[0] -= overall_msecs / 1000.0f;
	if (global_keys[GLFW_KEY_W])
		cameraPos[1] += overall_msecs / 1000.0f;
	else if (global_keys[GLFW_KEY_S])
		cameraPos[1] -= overall_msecs / 1000.0f;

	tick_otime = tick_ntime;
}

int main(void) {
	GLuint fieldTex;
	GLuint fieldFB;

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
	
	clamp_mode = 0;

	glGenTextures(1, &fieldTex);
	texturer->Bind(fieldTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glGenFramebuffers(1, &fieldFB);

	cameraPos[0] = cameraPos[1] = 0.0f;

	gettimeofday(&tick_otime, NULL);

	while ( !glfwWindowShouldClose(window) ) {
		float ox, oy, oz;

		glfwPollEvents();

		doTick();

		texturer->Bind(0);
		renderer->Begin();

		/*
		glTranslatef(cameraPos[0], -cameraPos[1], -8.0f);

		glRotatef(-45.0f, 1, 0, 0);
		glRotatef(45.0f, 0, 0, 1);
//		glRotatef(0.0f, 0, 1, 0);

//		glTranslatef(cameraPos[0], cameraPos[1], 0.0f);

*/
		renderer->SetTextureMode();

		// get into isometric view
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();

		double scale = 10;
		glOrtho(-scale,
			scale,
			-scale * 0.75,
			scale * 0.75,
			-scale,
			scale);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glLoadIdentity();

		glTranslatef(cameraPos[0], -cameraPos[1], 0.0f);
		glRotatef(-60.0f, 1, 0, 0);
		glRotatef(45.0f, 0, 0, 1);

		texturer->Bind(texturer->GetTile());
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(3.0f, 1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(3.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);
		glEnd();

		
		texturer->Bind(texturer->GetTestChar());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		{
			float cell = 16.0f / 512.0f;

			/*
			renderer->DrawSprite3D(0.5f, -0.5f, 1.0f, 1.0f, 2.0f, cell*2.0f, 0.0, cell, cell * 2, -45.0f);
			renderer->DrawSprite3D(0.5f, -0.5f, 1.0f, 1.0f, 2.0f, 0.0, 0.0, cell, cell * 2, -45.0f);
			renderer->DrawSprite3D(0.5f, -0.5f, 1.0f, 1.0f, 2.0f, cell, 0.0, cell, cell * 2, -45.0f);
			*/
			glPushMatrix();

			glTranslatef(0.5f, 0.5f, 1.0f);

			glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);

			// body
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, cell * 2.0f);
			glVertex3f(-0.5f, 0.0f, 1.0f);
			glTexCoord2f(cell, cell * 2.0f);
			glVertex3f(0.5f, 0.0f, 1.0f);
			glTexCoord2f(cell, cell * 4.0f);
			glVertex3f(0.5f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, cell * 4.0f);
			glVertex3f(-0.5f, 0.0f, -1.0f);
			glEnd();
			/*
			// upper leg
			glPushMatrix();
			glTranslatef(-0.3f, -0.01f, -1.0f);
			glRotatef(45.0f, 0.0f, 1.0f, 0.0f);

			glBegin(GL_QUADS);
			glTexCoord2f(cell, cell * 2.0f);
			glVertex3f(-0.5f, 0.0f, 1.0f);
			glTexCoord2f(cell * 2.0f, cell * 2.0f);
			glVertex3f(0.5f, 0.0f, 1.0f);
			glTexCoord2f(cell * 2.0f, cell * 4.0f);
			glVertex3f(0.5f, 0.0f, -1.0f);
			glTexCoord2f(cell, cell * 4.0f);
			glVertex3f(-0.5f, 0.0f, -1.0f);
			glEnd();

			// lower leg
			glPushMatrix();
			glTranslatef(0.0f, -0.01f, -1.0f);
			glRotatef(45.0f, 0.0f, 1.0f, 0.0f);

			glBegin(GL_QUADS);
			glTexCoord2f(cell * 2.0f, cell * 2.0f);
			glVertex3f(-0.5f, 0.0f, 1.0f);
			glTexCoord2f(cell * 3.0f, cell * 2.0f);
			glVertex3f(0.5f, 0.0f, 1.0f);
			glTexCoord2f(cell * 3.0f, cell * 4.0f);
			glVertex3f(0.5f, 0.0f, -1.0f);
			glTexCoord2f(cell * 2.0f, cell * 4.0f);
			glVertex3f(-0.5f, 0.0f, -1.0f);
			glEnd();


			// back
			glPopMatrix();
			glPopMatrix();
			*/
			glPopMatrix();

		}

		{
			renderer->ReadCoordsUnderCursor((int)global_mousepos[0], (int)global_mousepos[1], &ox, &oy, &oz);
		}
		

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		renderer->Set2DMode();
		/*
		// draw terrain
		texturer->Bind(texturer->GetWhite());
		{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(200.0f, 450.0f, 0.1f);
		glVertex3f(300.0f, 450.0f, 0.1f);
		glVertex3f(300.0f, 350.0f, 0.1f);
		glVertex3f(200.0f, 350.0f, 0.1f);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// draw character shape without DEPTH
		glDisable(GL_DEPTH_TEST);
		texturer->Bind(texturer->GetTest());
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PRIMARY_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		renderer->DrawTexturedRect(230.0f, 470.0f, 64.0f, 64.0f, 0.0, 0.0, 1.0f, 1.0f);

		// draw character with DEPTH
		texturer->Bind(texturer->GetTest());
		glEnable(GL_DEPTH_TEST);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		renderer->DrawTexturedRect(230.0f, 470.0f, 64.0f, 64.0f, 0.0, 0.0, 1.0f, 1.0f);
		*/

		// GUI
		glDisable(GL_DEPTH_TEST);
		texturer->Bind(texturer->GetWhite());
		renderer->DrawRect(100.0f, 668.0f, 100.0f, 32.0f, 0.0f, 0.0f, 0.0f, 0.1f);
		renderer->DrawBorder(100.0f, 668.0f, 100.0f, 32.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		gui->DrawString(102.0f, 666.0f, "Button", 16.0f, 2.0f);

		{
			char buf[1024];

			sprintf(buf, "OBJ: %.3f %.3f %.3f (%i %i)", ox, oy, oz,(int) global_mousepos[0], (int)global_mousepos[1]);
			gui->DrawString(0.0f, 760.0f, buf, 16.0f, 2.0f);

			sprintf(buf, "CAM: %.3f %.3f", cameraPos[0], cameraPos[1]);
			gui->DrawString(0.0f, 730.0f, buf, 16.0f, 2.0f);
		}

		gui->DrawCursor(global_mousepos[0], global_mousepos[1], 0);

		renderer->End2DMode();
		renderer->End();

		glfwSwapBuffers(window);       
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
