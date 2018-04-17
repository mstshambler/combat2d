#ifndef _gui_h
#define _gui_h

#include <string>
#include <list>

#include "common.h"
#include "render.h"
#include "texture.h"

class GUIElement {
protected:
	float pos[2];
	float size[2];
	byte align[2];
	byte enabled;
	GUIElement *parent;
	std::list<GUIElement *> *childs;
	byte type;

public:
	GUIElement();
	virtual ~GUIElement();

	enum GUIElementAlign {
		GUIElementAlign_LeftTop = 0,
		GUIElementAlign_RightBottom = 1,
		GUIElementAlign_Center = 2
	};

	enum GUIElementType {
		GUIElementType_None = 0
		GUIElementType_Text,
		GUIElementType_Button,
		GUIElementType_Checkbox,
		GUIElementType_List,
		GUIElementType_Combolist,
		GUIElementType_Edit,
		GUIElementType_Table,
		GUIElementType_TableRow,
		GUIElementType_Last
	};

	void SetPos(const float &x, const float &y);
	float GetPos(float *x, float *y) const;
	float GetPosX() const;
	float GetPosY() const;

	void SetSize(const float &width, const float &height);
	float GetSize(float *width, float *height) const;
	float GetSizeX() const;
	float GetSizeY() const;

	void SetAlign(const byte &x, const byte &y);
	byte GetAlign(byte *x, byte *y) const;
	byte GetAlignX() const;
	byte GetAlignY() const;

	void SetEnabled(const byte v);
	byte GetEnabled() const;

	void SetType(const byte t);
	byte GetType() const;

	void SetParent(GUIElement *elem);
	GUIElement *GetParent() const;

	void AddChild(GUIElement *elem);
	void RemoveChild(GUIElement *elem);
	std::list<GUIElement *> *GetChilds() const;

	virtual Render() const;
};

class GUIElementText : GUIElement {
protected:
	std::string *text;

public:
	GUIElementText();
	~GUIElementText();

	void SetText(std::string &t);
	std::string *GetText() const;
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