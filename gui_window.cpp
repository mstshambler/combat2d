#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUIElementWindow::GUIElementWindow() : GUIElement() {
	alpha = 0.0f;
}

GUIElementWindow::GUIElementWindow(const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, const float alpha, GUIElement *parent) : GUIElement(id, x, y, sizeX, sizeY, measureType, align, enabled, GUIElement::GUIElementType_Window, parent) {
	this->alpha = alpha;
}

GUIElementWindow::~GUIElementWindow() {
}

void GUIElementWindow::SetAlpha(const float &a) {
	alpha = a;
}

float GUIElementWindow::GetAlpha() const {
	return alpha;
}

void GUIElementWindow::RenderElement(Texture *texturer, Render *renderer, const byte &active, const byte &hover) const {
	texturer->Bind(texturer->GetWhite());
	renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), 0.0f, 0.0f, 0.0f, alpha);
}