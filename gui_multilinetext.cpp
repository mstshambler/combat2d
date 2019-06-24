#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUIElementMultilineText::GUIElementMultilineText() : GUIElementText() {
	scroll = 0.0f;
	maxScroll = 0.0f;
}

GUIElementMultilineText::GUIElementMultilineText(const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(id, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_MultilineText;
	scroll = 0.0f;
	maxScroll = 0.0f;
}


GUIElementMultilineText::~GUIElementMultilineText() {
}

void GUIElementMultilineText::SetScroll(const float &f) {
	scroll = f;
}

float GUIElementMultilineText::GetScroll() const {
	return scroll;
}

void GUIElementMultilineText::SetMaxScroll(const float &f) {
	maxScroll = f;
}

float GUIElementMultilineText::GetMaxScroll() const {
	return maxScroll;
}

void GUIElementMultilineText::RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const {
	texturer->Bind(texturer->GetWhite());
	renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), 0.0f, 0.0f, 0.0f, 0.8f);
	if (maxScroll > 0.0f) {
		float scrollPos = (float)(renderer->GetScreenHeight() - pixelPos.GetY());

		scrollPos -= (float)pixelSize.GetY() / 2.0f * (scroll / maxScroll);
		renderer->DrawRect((float)(pixelPos.GetX() + pixelSize.GetX()), scrollPos, 16.0f, (float)pixelSize.GetY() / 2.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	renderer->DrawStringBox(texturer, pixelPos.GetX() + 8.0f, renderer->GetScreenHeight() - pixelPos.GetY() - 8.0f, pixelSize.GetX() - 16.0f, pixelSize.GetY() - 16.0f,
		pixelPos.GetX() + 8.0f, renderer->GetScreenHeight() - pixelPos.GetY() + scroll - 8.0f, textSize, L"arial.ttf", text);

}

void GUIElementMultilineText::UpdateSize(Render *renderer) {
	float contentHeight = renderer->GetStringBoxSize(pixelSize.GetX() - 16.0f, pixelSize.GetY() - 16.0f, textSize, L"arial.ttf", text);

	if (contentHeight > pixelSize.GetY() - 16.0f) {
		maxScroll = contentHeight - (pixelSize.GetY() - 16.0f);
	}
}
