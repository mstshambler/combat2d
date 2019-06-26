#include "gui.h"

GUIElementEdit::GUIElementEdit(Texturer *texturer, Render *renderer) : GUIElementText(texturer, renderer) {
	type = GUIElement::GUIElementType_Edit;
	cursorPos = 0;
}

GUIElementEdit::GUIElementEdit(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize,
	const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType, const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(texturer, renderer,
	id, text, textSize, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_Edit;
	cursorPos = text.length();
}

GUIElementEdit::~GUIElementEdit() {
}

void GUIElementEdit::SetCursorPos(const int &i) {
	cursorPos = i;
}

int GUIElementEdit::GetCursorPos() const {
	return cursorPos;
}

void GUIElementEdit::RenderElement(const byte &active, const byte &hover) const {
	texturer->Bind(texturer->GetWhite());

	if (active) {
		float cursorPixelPos;
		float alpha;

		renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), 0.0f, 0.0f, 0.0f, 0.8f);

		alpha = (renderer->GetTickTime()->tv_sec % 2) * 1000.0f + renderer->GetTickTime()->tv_usec / 1000.0f;
		alpha = alpha / 1000.0f;
		if (alpha > 1.0f)
			alpha = 2.0f - alpha;

		cursorPixelPos = renderer->GetStringLength(textSize, L"arial.ttf", text, cursorPos);
		renderer->DrawRect((float)pixelPos.GetX() + cursorPixelPos - scroll, (float)(renderer->GetScreenHeight() - pixelPos.GetY()), 2.0f, (float)pixelSize.GetY(), 1.0f, 1.0f, 1.0f, alpha);
	} else if (hover)
		renderer->DrawRect((float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(), 0.0f, 0.0f, 0.0f, 0.8f);
	renderer->DrawStringBox(texturer, (float)pixelPos.GetX(), (float)(renderer->GetScreenHeight() - pixelPos.GetY()), (float)pixelSize.GetX(), (float)pixelSize.GetY(),
		0, (float)pixelPos.GetX() - scroll, (float)(renderer->GetScreenHeight() - pixelPos.GetY()), textSize, L"arial.ttf", text);
}

byte GUIElementEdit::DoActionClick(const int &x, const int &y) {
	scroll = maxScroll;
	cursorPos = text.length();
	return 1;
}

byte GUIElementEdit::DoActionKeyPress(const int &key, const int &scancode, const int &mod) {
	int maxLen = text.length();

	if (key == GLFW_KEY_LEFT && cursorPos > 0) {
		cursorPos--;
		ModifyScroll(-1);
	} else if (key == GLFW_KEY_RIGHT && cursorPos < maxLen) {
		cursorPos++;
		ModifyScroll(1);
	}  else if (key == GLFW_KEY_BACKSPACE && cursorPos > 0) {
		text.erase(cursorPos-1, 1);
		cursorPos--;
		UpdateSize();
		ModifyScroll(-1);
	} else if (key == GLFW_KEY_DELETE && cursorPos < maxLen) {
		text.erase(cursorPos, 1);
		UpdateSize();
	}

	return 1;
}

byte GUIElementEdit::DoActionCharPress(const unsigned int &keycode) {
	text.insert(cursorPos, 1, keycode);
	cursorPos++;
	UpdateSize();
	ModifyScroll(1);
	return 1;
}

byte GUIElementEdit::DoActionLoseFocus() {
	scroll = 0;
	return 1;
}

void GUIElementEdit::ModifyScroll(const int &shift) {
	float cursorPixelPos = renderer->GetStringLength(textSize, L"arial.ttf", text, cursorPos);

	if (cursorPixelPos < scroll)
		scroll = cursorPixelPos;
	else if (cursorPixelPos + 2 > pixelSize.GetX() + scroll)
		scroll = cursorPixelPos - pixelSize.GetX() + 2;
	if (scroll < 0)
		scroll = 0;
}
