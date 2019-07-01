#include "gui.h"

GUIElementText::GUIElementText(Texturer *texturer, Render *renderer) : GUIElement(texturer, renderer) {
	text = L"";
	textSize = Render::FontSize_Small;
	scroll = 0.0f;
	maxScroll = 0.0f;
	type = GUIElement::GUIElementType_Text;
}

GUIElementText::GUIElementText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElement(texturer, renderer, id, x, y, sizeX, sizeY, measureType, align, enabled, GUIElement::GUIElementType_Text, parent) {
	this->text = text;
	this->textSize = textSize;
	scroll = 0.0f;
	maxScroll = 0.0f;
}

GUIElementText::~GUIElementText() {
}

void GUIElementText::SetText(const wstring &t) {
	text = t;
	UpdateSize();
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

void GUIElementText::SetScroll(const float &f) {
	scroll = f;
}

float GUIElementText::GetScroll() const {
	return scroll;
}

void GUIElementText::SetMaxScroll(const float &f) {
	maxScroll = f;
}

float GUIElementText::GetMaxScroll() const {
	return maxScroll;
}

void GUIElementText::UpdateSize() {
	float contentWidth = renderer->GetStringLength(textSize, L"arial.ttf", text);

	if (contentWidth > pixelSize.GetX())
		maxScroll = contentWidth - pixelSize.GetX();
	else
		maxScroll = 0;
}

void GUIElementText::RenderElement(const byte &active, const byte &hover) const {
	renderer->DrawStringBox(texturer, (float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(),
		0, (float)pixelPos.GetX() - scroll, (float)(renderer->GetScreenHeight() - pixelPos.GetY()), textSize, L"arial.ttf", text, Color(1.0f, 1.0f, 1.0f, 1.0f));
}