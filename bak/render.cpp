#define _USE_MATH_DEFINES

#ifndef WIN32
#include <sys/time.h>
#endif

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "render.h"

Render::Render() {
	screenWidth = 0;
	screenHeight = 0;
}

int Render::GetScreenWidth() const {
	return screenWidth;
}

int Render::GetScreenHeight() const {
	return screenHeight;
}

void Render::Reshape(const int &width, const int &height) {
	screenWidth = width;
	screenHeight = height;
}

void Render::Init(const int &width, const int &height) {
	int		x, x2, y2, y;
	double screenaspect;

	// define screen resolution
	screenWidth = width;
	screenHeight = height;
	screenWidth2D = width;
	screenHeight2D = height;

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

void Render::Begin(int width, int height) const {
	double screenaspect;

	if (width == -1)
		width = screenWidth;
	if (height == -1)
		height = screenHeight;

	Render::Clear();

	glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, width, height);
    screenaspect = (double)width/height;
	gluPerspective (90.0f/screenaspect,  screenaspect,  1,  4096);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();

	glColor4f(1.0, 1.0, 1.0, 1.0);
}


void Render::SetTextureMode() const {
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

void Render::End() const {
	glRenderMode(GL_RENDER);
	glDisable(GL_DEPTH_TEST);
	glDisable (GL_ALPHA_TEST);
}

void Render::Set2DMode(int width, int height, int ignoreStack) {
	if (width == -1)
		width = screenWidth;
	if (height == -1)
		height = screenHeight;

	screenWidth2D = width;
	screenHeight2D = height;

	glMatrixMode(GL_PROJECTION);
	if (!ignoreStack)
		glPushMatrix();
    glLoadIdentity();

	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	if (!ignoreStack)
		glPushMatrix();
    glLoadIdentity ();

	//glDisable (GL_DEPTH_TEST);
	glDisable (GL_CULL_FACE);
	glEnable (GL_ALPHA_TEST);

	glColor4f (1,1,1,1);
}

void Render::End2DMode(int ignoreStack) const {
	if (!ignoreStack) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

void Render::Shutdown(void) const {
}

void Render::Clear (void) const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render::DrawTexturedRect(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height) const {
	DrawSprite(x + width / 2, y - height / 2, width, height, tx, ty, t_width, t_height, 0);
}

void Render::DrawSprite(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle) const {
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

void Render::DrawSprite3D(const float &x, const float &y, const float &z, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(tx, ty);
	glVertex3f(-width/2.0f, 0.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty);
	glVertex3f(width/2.0f, 0.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty + t_height);
	glVertex3f(width/2.0f, 0.0f, -height/2.0f);

	glTexCoord2f(tx, ty + t_height);
	glVertex3f(-width/2.0f, 0.0f, -height/2.0f);
	glEnd();
	glPopMatrix();
}

void Render::ConvertWindowToScreenCoords(const float &x, const float &y, float &sx, float &sy) const {
	sx = x * screenWidth2D / screenWidth;
	sy = screenHeight2D - 1.0f - y * screenHeight2D / screenHeight;
}

void Render::DrawBorder(const float &x, const float &y, const float &width, const float &height, const float &size, const float &r, const float &g, const float &b, const float &a) const {
	glLineWidth(size);
	glColor4f(r, g, b, a);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - height);
	glVertex2f(x, y - height);
	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Render::DrawRect(const float &x, const float &y, const float &width, const float &height, const float &r, const float &g, const float &b, const float &a) const {
	glColor4f(r, g, b, a);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - height);
	glVertex2f(x, y - height);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Render::ReadCoordsUnderCursor(const int &x, const int &y, float *ox, float *oy, float *oz) {
	GLfloat wy, wz;
	GLdouble objpos[3];
	GLdouble model_view[16];
	GLdouble projection[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glReadBuffer(GL_FRONT);
	wy = (float)viewport[3] - (float)y;
	glReadPixels(x, wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	gluUnProject(x, wy, wz,
		model_view, projection, viewport,
		&objpos[0], &objpos[1], &objpos[2]);

	objpos[1] *= -1;
	glReadBuffer(GL_BACK);

	if (ox)
		*ox = (float)objpos[0];
	if (oy)
		*oy = (float)objpos[1];
	if (oz)
		*oz = (float)objpos[2];
}
