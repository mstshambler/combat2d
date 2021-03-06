#include "gui.h"

GUIElementButton::GUIElementButton(Texturer *texturer, Render *renderer) : GUIElementText(texturer, renderer) {
	type = GUIElement::GUIElementType_Button;
}

GUIElementButton::GUIElementButton(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(texturer, renderer, id, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_Button;
}


GUIElementButton::~GUIElementButton() {
}

void GUIElementButton::RenderElement(const byte &active, const byte &hover) const {
	float sx, sy;

	sx = renderer->GetStringLength(textSize, L"arial.ttf", text);
	sy = renderer->GetStringHeight(textSize, L"arial.ttf", text);

	if (hover) {
		float alpha;

		texturer->Bind(texturer->GetWhite());
		alpha = (renderer->GetTickTime()->tv_sec % 2) * 1000.0f + renderer->GetTickTime()->tv_usec / 1000.0f;
		alpha = alpha / 1000.0f;
		if (alpha > 1.0f)
			alpha = 2.0f - alpha;
		alpha = 1.0f - 0.6f * alpha;

		renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), Color(0.0f, 0.0f, 0.0f, alpha));
	}

	sx = pixelPos.GetX() + pixelSize.GetX() / 2 - sx / 2;
	sy = pixelPos.GetY() + pixelSize.GetY() / 2 - sy / 2;
	//renderer->DrawString(texturer, sx, renderer->GetScreenHeight() - sy, textSize, L"arial.ttf", text);
	renderer->DrawStringBox(texturer, (float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(),
		0, sx, (float)(renderer->GetScreenHeight() - sy), textSize, L"arial.ttf", text, Color(1.0f, 1.0f, 1.0f, 1.0f));
}