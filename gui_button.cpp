#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUIElementButton::GUIElementButton() : GUIElementText() {
	type = GUIElement::GUIElementType_Button;
}

GUIElementButton::GUIElementButton(const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(id, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_Button;
}


GUIElementButton::~GUIElementButton() {
}

void GUIElementButton::RenderElement(Texture *texturer, Render *renderer, const byte &hover) const {
	float sx, sy;

	sx = renderer->GetStringLength(textSize, L"arial.ttf", text);
	sy = renderer->GetStringHeight(textSize, L"arial.ttf", text);

	if (hover) {
		texturer->Bind(texturer->GetWhite());
		renderer->DrawRect(pixelPos.GetX(), renderer->GetScreenHeight() - pixelPos.GetY(), pixelSize.GetX(), pixelSize.GetY(), 0.0f, 0.0f, 0.0f, 0.8f);
	}

	sx = pixelPos.GetX() + pixelSize.GetX() / 2 - sx / 2;
	sy = pixelPos.GetY() + pixelSize.GetY() / 2 - sy / 2;
	renderer->DrawString(texturer, sx, renderer->GetScreenHeight() - sy, textSize, L"arial.ttf", text);
}