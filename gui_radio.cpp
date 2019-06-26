#include "gui.h"

GUIElementRadio::GUIElementRadio(Texturer *texturer, Render *renderer) : GUIElementCheckbox(texturer, renderer) {
	type = GUIElement::GUIElementType_Radio;
	checked = 0;
}

GUIElementRadio::GUIElementRadio(Texturer *texturer, Render *renderer, const wstring &groupId, const wstring &id, const byte &checked, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementCheckbox(texturer, renderer, id, checked, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_Radio;
	this->groupId = groupId;
}

GUIElementRadio::~GUIElementRadio() {
}

void GUIElementRadio::SetGroupId(const wstring &s) {
	groupId = s;
}

wstring *GUIElementRadio::GetGroupId() const {
	return (wstring *)&groupId;
}

/*
void GUIElementRadio::RenderElement(const byte &active, const byte &hover) const {
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
*/

byte GUIElementRadio::DoActionClick(const int &x, const int &y) {
	if (parent) {
		list<GUIElement *>::iterator ei;

		for (ei = parent->GetChilds()->begin(); ei != parent->GetChilds()->end(); ei++) {
			GUIElement *e;

			e = *ei;
			if (e->GetType() == GUIElement::GUIElementType_Radio) {
				GUIElementRadio *er;

				er = (GUIElementRadio *)e;
				if (er != this && er->GetGroupId()->c_str() == groupId)
					er->SetChecked(0);
			}
		}
	}
	checked = 1;

	return 1;
}
