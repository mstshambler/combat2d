#ifndef _texture_h
#define _texture_h

#include "common.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class Texturer {
public:
	Texturer();
	~Texturer();

	void LoadTexturesList();
	int LoadTexture(const wstring &name, GLuint &id);
	void Bind(const GLuint texnum);

	GLuint GetWhite() const;
	GLuint GetGUIInterface() const;
	GLuint GetGUICursors() const;

	GLuint GetTest() const;

	GLuint GetTile() const;
	GLuint GetTestChar() const;

protected:
	GLuint texnum_white;
	GLuint texnum_current;
	GLuint texnum_GUICursors;
	GLuint texnum_GUIInterface;

	GLuint texnum_test;

	GLuint texnum_tile;
	GLuint texnum_testChar;
};

#endif