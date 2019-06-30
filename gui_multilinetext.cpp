#include "gui.h"

GUIElementMultilineText::GUIElementMultilineText(Texturer *texturer, Render *renderer) : GUIElementText(texturer, renderer) {
	type = GUIElement::GUIElementType_MultilineText;
	scrollHit = 0;
}

GUIElementMultilineText::GUIElementMultilineText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(texturer, renderer, id, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_MultilineText;
	scrollHit = 0;
}

GUIElementMultilineText::~GUIElementMultilineText() {
}

void GUIElementMultilineText::SetText(const wstring &t) {
	text = t;
	UpdateSize();
}

void GUIElementMultilineText::RenderElement(const byte &active, const byte &hover) const {
	float sx = pixelSize.GetX() - 16.0f;

	texturer->Bind(texturer->GetWhite());
	renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), Color(0.0f, 0.0f, 0.0f, 0.8f));

	if (maxScroll > 0.0f) {
		float scrollPos = (float)(renderer->GetScreenHeight() - pixelPos.GetY());

		scrollPos -= (float)pixelSize.GetY() / 2.0f * (scroll / maxScroll);
		renderer->DrawRect((float)(pixelPos.GetX() + pixelSize.GetX()) - 16.0f, scrollPos, 16.0f, (float)pixelSize.GetY() / 2.0f, Color(1.0f, 1.0f, 1.0f, 1.0f));
		sx -= 16.0f;
	}

	renderer->DrawStringBox(texturer, pixelPos.GetX() + 8.0f, renderer->GetScreenHeight() - pixelPos.GetY() - 8.0f, sx, pixelSize.GetY() - 16.0f,
		1, pixelPos.GetX() + 8.0f, renderer->GetScreenHeight() - pixelPos.GetY() + scroll - 8.0f, textSize, L"arial.ttf", text, Color(1.0f, 1.0f, 1.0f, 1.0f));

}

void GUIElementMultilineText::UpdateSize() {
	float contentHeight = renderer->GetStringHeight(textSize, L"arial.ttf", text, pixelSize.GetX() - 16.0f);

	if (contentHeight > pixelSize.GetY() - 16.0f) {
		contentHeight = renderer->GetStringHeight(textSize, L"arial.ttf", text, pixelSize.GetX() - 32.0f);
		maxScroll = contentHeight - (pixelSize.GetY() - 16.0f);
	}
}

byte GUIElementMultilineText::DoActionHold(const int &x, const int &y) {
	if (maxScroll > 0.0f) {
		if (scrollHit == 0) {
			if (x >= pixelPos.GetX() + pixelSize.GetX() - 16.0f && x <= pixelPos.GetX() + pixelSize.GetX())
				scrollHit = 1;
			else
				scrollHit = 2;
		}
		if (scrollHit == 1) {
			if (y < pixelPos.GetY() + pixelSize.GetY() / 4.0f)
				scroll = 0.0f;
			else if (y > pixelPos.GetY() + pixelSize.GetY() - pixelSize.GetY() / 4.0f)
				scroll = maxScroll;
			else
				scroll = (y - (pixelPos.GetY() + pixelSize.GetY() / 4.0f)) * maxScroll / (pixelSize.GetY() / 2.0f);
		}
	}
	return 1;
}

byte GUIElementMultilineText::DoActionStopHold(const int &x, const int &y) {
	scrollHit = 0;
	return 1;
}

byte GUIElementMultilineText::DoActionScroll(const float &x, const float &y) {
	scroll += y;
	if (scroll < 0)
		scroll = 0;
	else if (scroll > maxScroll)
		scroll = maxScroll;
	return 1;
}