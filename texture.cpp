#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texturer::Texturer() {
	texnum_current = 0;
}

Texturer::~Texturer() {
}

void Texturer::Bind(GLuint texnum) {
	if (texnum_current == texnum)
		return;

	texnum_current = texnum;
	glBindTexture(GL_TEXTURE_2D, texnum);
}

int Texturer::LoadTexture(const wstring &name, GLuint &id) {
	int width;
	int height;
	int channels;
	int ok = 0;
	char *oldName;

	oldName = new char[name.length() + 2];
	wcstombs(oldName, name.c_str(), name.length());
	oldName[name.length()] = 0;

	byte *bp = stbi_load(oldName, &width, &height, &channels, 0);
	if (bp) {
		glGenTextures(1, &id);
		Bind(id);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bp);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bp);

		ok = 1;
	}
	stbi_image_free(bp);

	delete oldName;
	return ok;
}

void Texturer::LoadTexturesList() {
	if (LoadTexture(L"data/textures/gui/white.tga", texnum_white) == 0)
		printf("Failed to load white texture\n");
	if (LoadTexture(L"data/textures/gui/cursors.tga", texnum_GUICursors) == 0)
		printf("Failed to load cursors\n");
	if (LoadTexture(L"data/textures/gui/interface.tga", texnum_GUIInterface) == 0)
		printf("Failed to load interface\n");
	
	if (LoadTexture(L"data/textures/test.png", texnum_test) == 0)
		printf("Failed to load test\n");

	if (LoadTexture(L"data/textures/tiles/testtile.tga", texnum_tile) == 0)
		printf("Failed to load tile\n");

	if (LoadTexture(L"data/textures/testchar.tga", texnum_testChar) == 0)
		printf("Failed to load test char\n");
}

GLuint Texturer::GetGUICursors() const {
	return texnum_GUICursors;
}

GLuint Texturer::GetGUIInterface() const {
	return texnum_GUIInterface;
}

GLuint Texturer::GetWhite() const {
	return texnum_white;
}

GLuint Texturer::GetTest() const {
	return texnum_test;
}

GLuint Texturer::GetTile() const {
	return texnum_tile;
}

GLuint Texturer::GetTestChar() const {
	return texnum_testChar;
}
