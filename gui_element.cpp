#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUIElement::GUIElement(Texturer *texturer, Render *renderer) {
	align = 0;
	enabled = 0;
	parent = NULL;
	childs = new list<GUIElement *>();
	type = GUIElementType_None;
	id = L"";

	actionClick = NULL;
	actionDrag = NULL;
	actionKeyPress = NULL;

	this->texturer = texturer;
	this->renderer = renderer;
}

GUIElement::GUIElement(Texturer *texturer, Render *renderer,
	const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType, const byte &align, const byte &enabled, const byte &type, GUIElement *parent) {
	this->id = id;
	pos.Set(x, y);
	size.Set(sizeX, sizeY);
	this->measureType = measureType;
	this->align = align;
	this->enabled = enabled;
	this->type = type;
	childs = new list<GUIElement *>();
	this->parent = parent;
	if (parent)
		parent->AddChild(this);

	actionClick = NULL;
	actionDrag = NULL;
	actionKeyPress = NULL;

	this->texturer = texturer;
	this->renderer = renderer;
}

GUIElement::~GUIElement() {
	delete childs;
}

Render *GUIElement::GetRenderer() {
	return renderer;
}

Texturer *GUIElement::GetTexturer() {
	return texturer;
}

PointInt *GUIElement::Pos() {
	return &pos;
}

PointInt *GUIElement::Size() {
	return &size;
}

PointInt *GUIElement::PixelPos() {
	return &pixelPos;
}

PointInt *GUIElement::PixelSize() {
	return &pixelSize;
}

void GUIElement::SetMeasureType(const byte &b) {
	measureType = b;
}

byte GUIElement::GetMeasureType() const {
	return measureType;
}

void GUIElement::SetAlign(const byte &b) {
	align = b;
}

byte GUIElement::GetAlign() const {
	return align;
}

void GUIElement::SetEnabled(const byte v) {
	enabled = v;
}

byte GUIElement::GetEnabled() const {
	return enabled;
}

void GUIElement::SetType(const byte t) {
	type = t;
}

byte GUIElement::GetType() const {
	return type;
}

void GUIElement::SetParent(GUIElement *elem) {
	parent = elem;
}

GUIElement *GUIElement::GetParent() const {
	return parent;
}

void GUIElement::AddChild(GUIElement *elem) {
	if (elem != NULL) {
		childs->push_back(elem);
		elem->SetParent(this);
	}
}

void GUIElement::RemoveChild(GUIElement *elem) {
	if (elem != NULL)
		childs->remove(elem);
}

list<GUIElement *> *GUIElement::GetChilds() {
	return childs;
}

void GUIElement::RenderElement(const byte &active, const byte &hover) const {
}

void GUIElement::SetActionClick(GUIElementClickAction a) {
	actionClick = a;
}

byte GUIElement::DoActionHold(const int &x, const int &y) {
	return 0;
}

byte GUIElement::DoActionStopHold(const int &x, const int &y) {
	return 0;
}

byte GUIElement::DoActionClick(const int &x, const int &y) {
	if (actionClick != NULL)
		return (*actionClick)(this, x - pixelPos.GetX(), y - pixelPos.GetY());
	return 0;
}

void GUIElement::SetActionDrag(GUIElementDragAction a) {
	actionDrag = a;
}

void GUIElement::SetActionKeyPress(GUIElementKeyAction a) {
	actionKeyPress = a;
}

byte GUIElement::DoActionKeyPress(const int &key, const int &scancode, const int &mod) {
	if (actionKeyPress != NULL)
		return (*actionKeyPress)(this, key, scancode, mod);
	return 0;
}

void GUIElement::SetId(const wstring &t) {
	id = t;
}

wstring *GUIElement::GetId() const {
	return (wstring *)&id;
}

GUIElement *GUIElement::FindElement(const wstring &id) {
	list<GUIElement *>::iterator ei;

	for (ei = childs->begin(); ei != childs->end(); ei++) {
		GUIElement *e = *ei;
		if (*(e->GetId()) == id)
			return e;
	}
	return NULL;
}

void GUIElement::UpdateSize() {
}

