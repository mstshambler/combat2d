#include "gui.h"

GUIElementWindow::GUIElementWindow(Texturer *texturer, Render *renderer) : GUIElement(texturer, renderer) {
	type = GUIElement::GUIElementType_Window;
	alpha = 0.0f;
}

GUIElementWindow::GUIElementWindow(Texturer *texturer, Render *renderer, const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, const float alpha, GUIElement *parent) : GUIElement(texturer, renderer, id, x, y, sizeX, sizeY, measureType, align, enabled, GUIElement::GUIElementType_Window, parent) {
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

void GUIElementWindow::RenderElement(const byte &active, const byte &hover) const {
	texturer->Bind(texturer->GetWhite());
	renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), Color(0.0f, 0.0f, 0.0f, alpha));
}