#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUIElementText::GUIElementText() : GUIElement() {
	text = L"";
	textSize = Render::FontSize_Small;
}

GUIElementText::GUIElementText(const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElement(id, x, y, sizeX, sizeY, measureType, align, enabled, GUIElement::GUIElementType_Text, parent) {
	this->text = text;
	this->textSize = textSize;
}


GUIElementText::~GUIElementText() {
}

void GUIElementText::SetText(const wstring &t) {
	text = t;
}

wstring *GUIElementText::GetText() const {
	return (wstring *)&text;
}

void GUIElementText::SetTextSize(const int &i) {
	textSize = i;
}

int GUIElementText::GetTextSize() const {
	return textSize;
}

void GUIElementText::RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const {
	renderer->DrawString(texturer, pixelPos.GetX(), renderer->GetScreenHeight() - pixelPos.GetY(), textSize, L"arial.ttf", text);
}