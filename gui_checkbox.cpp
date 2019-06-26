#include "gui.h"

GUIElementCheckbox::GUIElementCheckbox(Texturer *texturer, Render *renderer) : GUIElementText(texturer, renderer) {
	type = GUIElement::GUIElementType_Checkbox;
	checked = 0;
}

GUIElementCheckbox::GUIElementCheckbox(Texturer *texturer, Render *renderer, const wstring &id, const byte &checked, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(texturer, renderer, id, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_Checkbox;
	this->checked = checked;
}

GUIElementCheckbox::~GUIElementCheckbox() {
}

void GUIElementCheckbox::SetChecked(const byte &b) {
	checked = b;
}

byte GUIElementCheckbox::GetChecked() const {
	return checked;
}

void GUIElementCheckbox::RenderElement(const byte &active, const byte &hover) const {
	float radius;
	float px;
	float alpha;

	texturer->Bind(texturer->GetWhite());

	alpha = 1.0f;

	if (hover) {
		alpha = (renderer->GetTickTime()->tv_sec % 2) * 1000.0f + renderer->GetTickTime()->tv_usec / 1000.0f;
		alpha = alpha / 1000.0f;
		if (alpha > 1.0f)
			alpha = 2.0f - alpha;
	}

	radius = pixelSize.GetY() / 2.0f - 8.0f;
	renderer->DrawCircle(pixelPos.GetX() + radius + 4.0f, (float)(renderer->GetScreenHeight() - pixelPos.GetY() - pixelSize.GetY() / 2), radius, 0, 1.0f, 1.0f, 1.0f, alpha);
	if (checked)
		renderer->DrawCircle(pixelPos.GetX() + radius + 4.0f, (float)(renderer->GetScreenHeight() - pixelPos.GetY() - pixelSize.GetY() / 2), radius - 3.0f, 1, 1.0f, 1.0f, 1.0f, alpha);

	// don't forget to change padding size in GUI:ResizeElements!
	px = radius * 2.0f + 8.0f;
	renderer->DrawStringBox(texturer, (float)pixelPos.GetX() + px, (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX() - px, (float)pixelSize.GetY(),
		0, (float)pixelPos.GetX() + px, (float)(renderer->GetScreenHeight() - pixelPos.GetY()), textSize, L"arial.ttf", text);
}

byte GUIElementCheckbox::DoActionClick(const int &x, const int &y) {
	checked = 1 - checked;

	return 1;
}
