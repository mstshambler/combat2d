#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gui.h"

GUI::GUI(Texturer *t, Render *r) {
	texturer = t;
	renderer = r;

	zoom = 1.0f;

	rootElement = new GUIElement(t, r, L"root", 0, 0, renderer->GetScreenWidth(), renderer->GetScreenHeight(), 0, 0, 0, GUIElement::GUIElementType_None, NULL);
	activeElement = halfActiveElement = hoverElement = NULL;
	Resize();
}

GUI::~GUI() {
	if (rootElement)
		delete rootElement;
}

void GUI::DrawCursor(const float x, const float y, const int mode) const {
	float local_mousepos[2];
	float cell;

	renderer->ConvertWindowToScreenCoords(x, y, local_mousepos[0], local_mousepos[1]);
	cell = 32.0f / 128.0f;
			
	texturer->Bind(texturer->GetGUICursors());
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	renderer->DrawSprite(local_mousepos[0], local_mousepos[1], 32.0f, 32.0f, 0.0f, 0.0f, cell, cell, 0.0f);
}

float GUI::GetZoom() const {
	return zoom;
}

GUIElement *GUI::GetRootElement() {
	return rootElement;
}

void GUI::Resize() {
	renderer->InitFonts(zoom);
	rootElement->PixelSize()->Set(renderer->GetScreenWidth(), renderer->GetScreenHeight());
	ResizeElements(rootElement);
}

void GUI::Resize(const float &f) {
	zoom = f;
	Resize();
}

void GUI::ResizeElements(GUIElement *root) {
	GUIElement *e;
	list<GUIElement *>::iterator li;

	for (li = root->GetChilds()->begin(); li != root->GetChilds()->end(); li++) {
		int px, py, sx, sy;

		e = *li;
		
		if (e->GetMeasureType() & GUIElement::GUIElementMeasureType_PercentSizeX)
			sx = e->Size()->GetX() * root->PixelSize()->GetX() / 100;
		else if (e->GetMeasureType() & GUIElement::GUIElementMeasureType_ContentSizeX) {
			if (e->GetType() == GUIElement::GUIElementType_Text) {
				GUIElementText *te = (GUIElementText *)e;
				sx = (int)renderer->GetStringLength(te->GetTextSize(), L"arial.ttf", *(te->GetText()));
			} else
				sx = 0;
		} else
			sx = (int)( (float)e->Size()->GetX() * zoom);
		if (e->GetMeasureType() & GUIElement::GUIElementMeasureType_PercentSizeY)
			sy = e->Size()->GetY() * root->PixelSize()->GetY() / 100;
		else if (e->GetMeasureType() & GUIElement::GUIElementMeasureType_ContentSizeY) {
			if (e->GetType() == GUIElement::GUIElementType_Text) {
				GUIElementText *te = (GUIElementText *)e;
				sy = (int)renderer->GetStringHeight(te->GetTextSize(), L"arial.ttf", *(te->GetText()));
			} else if (e->GetType() == GUIElement::GUIElementType_Edit) {
					GUIElementEdit *te = (GUIElementEdit *)e;
					sy = (int)renderer->GetStringHeight(te->GetTextSize(), L"arial.ttf", *(te->GetText()));
			} else
				sy = 0;
		} else
			sy = (int)((float)e->Size()->GetY() * zoom);
		e->PixelSize()->Set(sx, sy);

		if (e->GetAlign() & GUIElement::GUIElementAlign_HorCenter)
			px = root->PixelPos()->GetX() + root->PixelSize()->GetX() / 2 - sx / 2;
		else {
			if (e->GetMeasureType() & GUIElement::GUIElementMeasureType_PercentPosX) {
				px = root->PixelSize()->GetX() * e->Pos()->GetX() / 100;
			} else {
				px = (int) ((float)e->Pos()->GetX() * zoom);
			}
			if (e->GetAlign() & GUIElement::GUIElementAlign_Right)
				px = root->PixelPos()->GetX() + root->PixelSize()->GetX() - px - sx;
			else
				px = root->PixelPos()->GetX() + px;
		}
		if (e->GetAlign() & GUIElement::GUIElementAlign_VertCenter)
			py = root->PixelPos()->GetY() + root->PixelSize()->GetY() / 2 - sy / 2;
		else {
			if (e->GetMeasureType() & GUIElement::GUIElementMeasureType_PercentPosY) {
				py = root->PixelSize()->GetY() * e->Pos()->GetY() / 100;
			} else {
				py = (int)((float)e->Pos()->GetY() * zoom);
			}
			if (e->GetAlign() & GUIElement::GUIElementAlign_Bottom)
				py = root->PixelPos()->GetY() + root->PixelSize()->GetY() - py - sy;
			else
				py = root->PixelPos()->GetY() + py;
		}
		e->PixelPos()->Set(px, py);
		e->UpdateSize();

		ResizeElements(e);
	}	
}

void GUI::RenderElements(GUIElement *root, const float &mouseX, const float &mouseY) {
	GUIElement *e;
	list<GUIElement *>::iterator li;
	
	li = root->GetChilds()->begin();
	for (li = root->GetChilds()->begin(); li != root->GetChilds()->end(); li++) {
		e = *li;
		if (e->GetEnabled()) {
			e->RenderElement(activeElement == e, hoverElement == e);
			RenderElements(e, mouseX, mouseY);
		}
	}
}

GUIElement *GUI::FindElementByCoords(GUIElement *root, const float &mouseX, const float &mouseY) {
	GUIElement *e;
	list<GUIElement *>::iterator li;
	GUIElement *foundElement = NULL;
	
	for (li = root->GetChilds()->begin(); li != root->GetChilds()->end(); li++) {
		e = *li;
		if (e->GetEnabled()) {
			GUIElement *subFoundElement = NULL;
			subFoundElement = FindElementByCoords(e, mouseX, mouseY);
			if (subFoundElement)
				foundElement = subFoundElement;
			else if (mouseX >= e->PixelPos()->GetX() && mouseY >= e->PixelPos()->GetY() &&
				mouseX <= e->PixelPos()->GetX() + e->PixelSize()->GetX() && mouseY <= e->PixelPos()->GetY() + e->PixelSize()->GetY()) {
				foundElement = e;
			}
		}
	}
	return foundElement;
}

GUIElement *GUI::FindElement(const wstring &id) {
	return rootElement->FindElement(id);
}

void GUI::SetHoverElement(GUIElement *e) {
	hoverElement = e;
}

GUIElement *GUI::GetHoverElement() {
	return hoverElement;
}

void GUI::SetActiveElement(GUIElement *e) {
	activeElement = e;
}

GUIElement *GUI::GetActiveElement() {
	return activeElement;
}

void GUI::SetHalfActiveElement(GUIElement *e) {
	halfActiveElement = e;
}

GUIElement *GUI::GetHalfActiveElement() {
	return halfActiveElement;
}
