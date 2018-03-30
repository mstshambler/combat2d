#ifndef _gui_h
#define _gui_h

#include <string>

#include "common.h"
#include "render.h"
#include "texture.h"

class GUI {
protected:
	Texture *texturer;
	Render *renderer;
	byte fontCharSize[256];

public:
	GUI(Texture *t, Render *r);
	~GUI();

	void DrawString(const float x, const float y, const std::string &text, const float size, const float padding) const;
	void DrawCursor(const float x, const float y, const int mode) const;
};

#endif