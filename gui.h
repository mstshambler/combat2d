#ifndef _gui_h
#define _gui_h

#include "common.h"
#include "render.h"
#include "texture.h"

using namespace std;

typedef byte(*GUIElementClickAction)(void *, const int&, const int&);
typedef byte(*GUIElementDragAction)(void *, int, int, int, int);
typedef byte(*GUIElementKeyAction)(void *, const int&, const int&, const int&);

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
	Texturer *texturer;

	GUIElementClickAction actionClick;
	GUIElementDragAction actionDrag;
	GUIElementKeyAction actionKeyPress;

	wstring id;

public:
	GUIElement(Texturer *texturer, Render *renderer);
	GUIElement(Texturer *texturer, Render *renderer, const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
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
		GUIElementType_Radio,
		GUIElementType_List,
		GUIElementType_Combolist,
		GUIElementType_Edit,
		GUIElementType_Table,
		GUIElementType_TableRow,
		GUIElementType_TableItemText,
		GUIElementType_TableItemImage,
		GUIElementType_Window,
		GUIElementType_Last
	};

	Render *GetRenderer();
	Texturer *GetTexturer();

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

	virtual void RenderElement(const byte &active, const byte &hover) const;

	void SetActionClick(GUIElementClickAction a);
	void SetActionDrag(GUIElementDragAction a);
	void SetActionKeyPress(GUIElementKeyAction a);

	virtual byte DoActionHold(const int &x, const int &y);
	virtual byte DoActionStopHold(const int &x, const int &y);
	virtual byte DoActionClick(const int &x, const int &y);
	virtual byte DoActionKeyPress(const int &key, const int &scancode, const int &mod);
	virtual byte DoActionCharPress(const unsigned int &keycode);
	virtual byte DoActionScroll(const float &x, const float &y);
	virtual byte DoActionLoseFocus();

	virtual void UpdateSize();

	GUIElement *FindElement(const wstring &id);
};

class GUIElementWindow : public GUIElement {
protected:
	float alpha;

public:
	GUIElementWindow(Texturer *texturer, Render *renderer);
	GUIElementWindow(Texturer *texturer, Render *renderer, const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, const float alpha, GUIElement *parent);
	~GUIElementWindow();

	void SetAlpha(const float &a);
	float GetAlpha() const;

	void RenderElement(const byte &active, const byte &hover) const;
};

class GUIElementText : public GUIElement {
protected:
	wstring text;
	int textSize;
	float scroll;
	float maxScroll;

public:
	GUIElementText(Texturer *texturer, Render *renderer);
	GUIElementText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementText();

	virtual void SetText(const wstring &t);
	wstring *GetText() const;

	void SetTextSize(const int &i);
	int GetTextSize() const;

	void SetScroll(const float &f);
	float GetScroll() const;
	void SetMaxScroll(const float &f);
	float GetMaxScroll() const;

	void UpdateSize();

	void RenderElement(const byte &active, const byte &hover) const;
};

class GUIElementMultilineText : public GUIElementText {
protected:
	byte scrollHit;

public:
	GUIElementMultilineText(Texturer *texturer, Render *renderer);
	GUIElementMultilineText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementMultilineText();

	virtual void SetText(const wstring &t);

	void RenderElement(const byte &active, const byte &hover) const;

	void UpdateSize();

	byte DoActionHold(const int &x, const int &y);
	byte DoActionStopHold(const int &x, const int &y);
	byte DoActionScroll(const float &x, const float &y);
};

class GUIElementButton : public GUIElementText {
public:
	GUIElementButton(Texturer *texturer, Render *renderer);
	GUIElementButton(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementButton();

	void RenderElement(const byte &active, const byte &hover) const;
};

class GUIElementEdit : public GUIElementText {
protected:
	int cursorPos;

public:
	GUIElementEdit(Texturer *texturer, Render *renderer);
	GUIElementEdit(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementEdit();

	void SetCursorPos(const int &i);
	int GetCursorPos() const;

	void RenderElement(const byte &active, const byte &hover) const;

	byte DoActionClick(const int &x, const int &y);
	byte DoActionCharPress(const unsigned int &keycode);
	byte DoActionKeyPress(const int &key, const int &scancode, const int &mod);
	byte DoActionLoseFocus();

	void ModifyScroll(const int &shift);
};

class GUIElementCheckbox : public GUIElementText {
protected:
	byte checked;

public:
	GUIElementCheckbox(Texturer *texturer, Render *renderer);
	GUIElementCheckbox(Texturer *texturer, Render *renderer, const wstring &id, const byte &checked, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementCheckbox();

	void SetChecked(const byte &b);
	byte GetChecked() const;

	void RenderElement(const byte &active, const byte &hover) const;

	byte DoActionClick(const int &x, const int &y);
};

class GUIElementRadio : public GUIElementCheckbox {
protected:
	wstring groupId;

public:
	GUIElementRadio(Texturer *texturer, Render *renderer);
	GUIElementRadio(Texturer *texturer, Render *renderer, const wstring &groupId, const wstring &id, const byte &checked, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementRadio();

	void SetGroupId(const wstring &s);
	wstring *GetGroupId() const;

//	void RenderElement(const byte &active, const byte &hover) const;

	byte DoActionClick(const int &x, const int &y);
};

class GUIElementTableItemText : public GUIElementText {
public:
	GUIElementTableItemText(Texturer *texturer, Render *renderer);
	GUIElementTableItemText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &sizeX, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementTableItemText();
};

class GUIElementTableItemImage : public GUIElementTableItemText {
protected:
	GLuint tex;
	PointFloat texCoords;
	PointFloat texSize;

public:
	GUIElementTableItemImage(Texturer *texturer, Render *renderer);
	GUIElementTableItemImage(Texturer *texturer, Render *renderer, const wstring &id, const GLuint &tex, const float &tx, const float &ty, const float &tsx, const float &tsy,
		const int &sizeX, const byte &measureType, const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementTableItemImage();

	void SetTex(const GLuint &t);
	GLuint GetTex() const;

	PointFloat *TexCoords();
	PointFloat *TexSize();
};

class GUIElementTableRow : public GUIElement {
public:
	GUIElementTableRow(Texturer *texturer, Render *renderer);
	GUIElementTableRow(Texturer *texturer, Render *renderer, const wstring &id);
	~GUIElementTableRow();
};

class GUIElementTable : public GUIElementMultilineText {
protected:
	list<GUIElementTableRow *> *rows;
	GUIElementTableRow *header;

public:
	GUIElementTable(Texturer *texturer, Render *renderer);
	GUIElementTable(Texturer *texturer, Render *renderer, const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
		const byte &align, const byte &enabled, GUIElement *parent);
	~GUIElementTable();
	
	void RenderElement(const byte &active, const byte &hover) const;

	void UpdateSize();

	void AddRow(GUIElementTableRow *row);
	void DeleteRow(GUIElementTableRow *row);
	list<GUIElementTableRow *> *GetRows();

	GUIElementTableRow *GetHeader();
};

class GUI {
protected:
	Texturer *texturer;
	Render *renderer;
	GUIElement *rootElement;
	float zoom;

	GUIElement *activeElement; // an element we clicked on (left mouse button ON and OFF)
	GUIElement *halfActiveElement; // an element we half-clicked on (left mouse button ON)
	GUIElement *hoverElement; // an element we keep our mouse cursor over

public:
	GUI(Texturer *t, Render *r);
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