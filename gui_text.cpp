#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUIElementText::GUIElementText(Texturer *texturer, Render *renderer) : GUIElement(texturer, renderer) {
	text = L"";
	textSize = Render::FontSize_Small;
}

GUIElementText::GUIElementText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElement(texturer, renderer, id, x, y, sizeX, sizeY, measureType, align, enabled, GUIElement::GUIElementType_Text, parent) {
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

void GUIElementText::RenderElement(const byte &active, const byte &hover) const {
	renderer->DrawString(texturer, (float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), textSize, L"arial.ttf", text);
}