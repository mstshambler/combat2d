#ifndef _gui_h
#define _gui_h

#include "common.h"
#include "render.h"
#include "texture.h"

using namespace std;

typedef int(*GUIElementClickAction)(void *, const int&, const int&);
typedef int(*GUIElementDragAction)(void *, int, int, int, int);
typedef int(*GUIElementKeyAction)(void *, const int&, const int&, const int&);

class GUIElement {
protected:
	PointInt pos;
	PointInt pixelPos;
	PointInt size;
	PointInt pixelSize;
	byte measureType;
	byte align;
	byte enabled;
	GUIElement *parent;
	list<GUIElement *> *childs;
	byte type;
	Render *renderer;
	Texture *texturer;

	GUIElementClickAction actionClick;
	GUIElementDragAction actionDrag;
	GUIElementKeyAction actionKeyPress;

	wstring id;

public:
	GUIElement();
	GUIElement(const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, const byte &type, GUIElement *parent);
	virtual ~GUIElement();

	enum GUIElementMeasureType {
		GUIElementMeasureType_PercentPosX = 1,
		GUIElementMeasureType_PercentPosY = 2,
		GUIElementMeasureType_PercentSizeX = 4,
		GUIElementMeasureType_PercentSizeY = 8,
		GUIElementMeasureType_ContentSizeX = 16,
		GUIElementMeasureType_ContentSizeY = 32
	};

	enum GUIElementAlign {
		GUIElementAlign_HorCenter = 1,
		GUIElementAlign_Right = 2,
		GUIElementAlign_VertCenter = 4,
		GUIElementAlign_Bottom = 8,
	};

	enum GUIElementType {
		GUIElementType_None = 0,
		GUIElementType_Text,
		GUIElementType_MultilineText,
		GUIElementType_Button,
		GUIElementType_Checkbox,
		GUIElementType_List,
		GUIElementType_Combolist,
		GUIElementType_Edit,
		GUIElementType_Table,
		GUIElementType_TableRow,
		GUIElementType_Window,
		GUIElementType_Last
	};

	PointInt *Pos();
	PointInt *Size();
	PointInt *PixelPos();
	PointInt *PixelSize();

	void SetId(const wstring &t);
	wstring *GetId() const;

	void SetMeasureType(const byte &b);
	byte GetMeasureType() const;

	void SetAlign(const byte &b);
	byte GetAlign() const;

	void SetEnabled(const byte v);
	byte GetEnabled() const;

	void SetType(const byte t);
	byte GetType() const;

	void SetParent(GUIElement *elem);
	GUIElement *GetParent() const;

	void AddChild(GUIElement *elem);
	void RemoveChild(GUIElement *elem);
	list<GUIElement *> *GetChilds();

	virtual void RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const;

	void SetActionClick(GUIElementClickAction a);
	void SetActionDrag(GUIElementDragAction a);
	void SetActionKeyPress(GUIElementKeyAction a);

	virtual void DoActionClick(const int &x, const int &y);
	virtual void DoActionKeyPress(const int &key, const int &scancode, const int &mod);

	virtual void UpdateSize(Render *renderer);

	GUIElement *FindElement(const wstring &id);
};

class GUIElementWindow : public GUIElement {
protected:
	float alpha;

public:
	GUIElementWindow();
	GUIElementWindow(const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, const float alpha, GUIElement *parent);
	~GUIElementWindow();

	void SetAlpha(const float &a);
	float GetAlpha() const;

	void RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const;
};

class GUIElementText : public GUIElement {
protected:
	wstring text;
	int textSize;

public:
	GUIElementText();
	GUIElementText(const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementText();

	virtual void SetText(const wstring &t);
	wstring *GetText() const;

	void SetTextSize(const int &i);
	int GetTextSize() const;

	void RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const;
};

class GUIElementMultilineText : public GUIElementText {
protected:
	float scroll;
	float maxScroll;

public:
	GUIElementMultilineText();
	GUIElementMultilineText(const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementMultilineText();

	void SetScroll(const float &f);
	float GetScroll() const;
	void SetMaxScroll(const float &f);
	float GetMaxScroll() const;

	void RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const;

	void UpdateSize(Render *renderer);
};

class GUIElementButton : public GUIElementText {
public:
	GUIElementButton();
	GUIElementButton(const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementButton();

	void RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const;
};


class GUI {
protected:
	Texture *texturer;
	Render *renderer;
	GUIElement *rootElement;
	float zoom;

	GUIElement *activeElement; // an element we clicked on (left mouse button ON and OFF)
	GUIElement *halfActiveElement; // an element we half-clicked on (left mouse button ON)
	GUIElement *hoverElement; // an element we keep our mouse cursor over

public:
	GUI(Texture *t, Render *r);
	~GUI();

	void ResizeElements(GUIElement *root);
	void Resize();
	void Resize(const float &f);
	float GetZoom() const;

	GUIElement *GetRootElement();

	void DrawCursor(const float x, const float y, const int mode) const;
	void RenderElements(GUIElement *root, const float &mouseX, const float &mouseY);
	GUIElement *FindElementByCoords(GUIElement *root, const float &mouseX, const float &mouseY);

	GUIElement *FindElement(const wstring &id);

	void SetHoverElement(GUIElement *e);
	GUIElement *GetHoverElement();
	void SetActiveElement(GUIElement *e);
	GUIElement *GetActiveElement();
	void SetHalfActiveElement(GUIElement *e);
	GUIElement *GetHalfActiveElement();
};

#endif