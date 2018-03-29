#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#ifndef WIN32
#include <sys/time.h>
#endif

#ifdef WIN32
#include <GL/glew.h>
#endif

#include <GL/glut.h>

#include "common.h"

#ifdef WIN32
#include "gettimeofday.h"
#endif

#include "math.h"
#include "render.h"
#include "texture.h"

Render::Render() {
	screen_width = 0;
	screen_height = 0;
}

int Render::GetScreenWidth() {
	return screen_width;
}

int Render::GetScreenHeight() {
	return screen_height;
}

void Render::Reshape(int width, int height) {
	screen_width = width;
	screen_height = height;
}

void Render::Init(int width, int height) {
	int		x, x2, y2, y;
	double screenaspect;

	// define screen resolution
	screen_width = width;
	screen_height = height;

	x = 0;
	x2 = width;
	y = height;
	y2 = 0;

	// Set default OpenGL settings
	glClearColor( 0.0F, 0.0F, 0.0F, 0.0F );
	glClearStencil(0);
	glShadeModel( GL_SMOOTH );

	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    screenaspect = (double)width/height;
	gluPerspective (90.0f/screenaspect,  screenaspect,  0,  4096);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glViewport (x, y2, width, height);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	glDisable(GL_LIGHT0);
}

void Render::Begin(int width, int height) {
	double screenaspect;

	if (width == -1)
		width = screen_width;
	if (height == -1)
		height = screen_height;

	Render::Clear();

	glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, width, height);
    screenaspect = (double)width/height;
	gluPerspective (90.0f/screenaspect,  screenaspect,  1,  4096);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();

	// Make Z look up (first person view)
	glRotatef(-90, 1, 0, 0);
//	glRotatef(90, 0, 0, 1);

	glRotatef(0.0f,  1, 0, 0);
	glRotatef(0.0f, 0, 1, 0);
	glRotatef(0.0f, 0, 0, 1);

	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor4f(1.0, 1.0, 1.0, 1.0);
}


void Render::SetTextureMode() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthRange(0, 1);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glEnable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Render::End() {
	glRenderMode(GL_RENDER);
	glDisable(GL_DEPTH_TEST);
	glDisable (GL_ALPHA_TEST);

	glutSwapBuffers();
}

void Render::Set2DMode(int width, int height) {
	if (width == -1)
		width = screen_width;
	if (height == -1)
		height = screen_height;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
    glLoadIdentity ();

	glDisable (GL_DEPTH_TEST);
	glDisable (GL_CULL_FACE);
	glEnable (GL_ALPHA_TEST);

	glColor4f (1,1,1,1);
}

void Render::End2DMode() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Render::Shutdown(void) {
}

void Render::Clear (void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render::DrawSprite(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(tx, ty);
	glVertex2f(-width/2.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty);
	glVertex2f(width/2.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty + t_height);
	glVertex2f(width/2.0f, -height/2.0f);

	glTexCoord2f(tx, ty + t_height);
	glVertex2f(-width/2.0f, -height/2.0f);
	glEnd();
	glPopMatrix();
}
