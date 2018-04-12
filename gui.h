#ifndef _gui_h
#define _gui_h

#include <string>

#include "common.h"
#include "render.h"
#include "texture.h"

class GUIElement {
protected:
	float pos[2];
	float size[2];
	byte align[2];
	byte enabled;

public:
	GUIElement();
	~GUIElement();

	enum GUIElementAlign {
		GUIElementAlign_LeftTop = 0,
		GUIElementAlign_RightBottom = 1,
		GUIElementAlign_Center = 2
	};

	virtual void SetPos(const float &x, const float &y);
	virtual float GetPos(float *x, float *y) const;
	virtual float GetPosX() const;
	virtual float GetPosY() const;

	virtual void SetSize(const float &width, const float &height);
	virtual float GetSize(float *width, float *height) const;
	virtual float GetSizeX() const;
	virtual float GetSizeY() const;

	virtual void SetAlign(const byte &x, const byte &y);
	virtual byte GetAlign(byte *x, byte *y) const;
	virtual byte GetAlignX() const;
	virtual byte GetAlignY() const;

	virtual byte SetEnabled(const byte v);
	virtual byte GetEnabled() const;
};

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