#ifndef _texture_h
#define _texture_h

#include "common.h"

class Texture {
public:
	Texture();
	~Texture();

	void LoadTexturesList();
	int LoadTexture(const std::string &name, GLuint &id);
	void Bind(const GLuint texnum);

	GLuint GetGUICursors() const;
	GLuint GetFontFixed() const;

protected:
	GLuint texnum_current;
	GLuint texnum_fontFixed;
	GLuint texnum_GUICursors;
};

#endif