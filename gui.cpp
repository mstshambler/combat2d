#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gui.h"

GUI::GUI(Texture *t, Render *r) {
	FILE *fp;
	char buf[256];
	int i;

	texturer = t;
	renderer = r;

	// load font size
	fp = fopen("data/fontsize.cfg", "r");
	if (fp) {
		for (i = 0; i < 256; i++) {
			fgets(buf, 250, fp);
			fontCharSize[i] = (byte)atoi(buf);
		}
		fclose(fp);
	} else {
		for (i = 0; i < 256; i++) {
			fgets(buf, 250, fp);
			fontCharSize[i] = 16;
		}
		printf("Can't open fontsize.cfg\n");
	}
}

GUI::~GUI() {
}

void GUI::DrawString(const float x, const float y, const std::string &text, const float size, const float padding) const {
	char *c;
	float px;
	float cell;

	texturer->Bind(texturer->GetFontFixed());
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	cell = 16.0f / 512.0f;
	c = (char *)text.c_str();
	px = x;
	
	while(*c) {
		renderer->DrawTexturedRect(px, y, size, size, (*c % 32) * cell, floor(*c / 32.0f) * cell, cell, cell);
		px += (float)fontCharSize[*c] * (size / 16.0f) + padding;
		c++;
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GUIElement::GUIElement() {
	pos[0] = pos[1] = 0.0f;
	size[0] = size[1] = 0.0f;
	align[0] = align[1] = GUIElementAlign_LeftTop;
	enabled = 0;
	parent = NULL;
	childs = new std::list<GUIElement *>();
	type = GUIElementType_None;
}

GUIElement::~GUIElement() {
	delete childs;
}

void GUIElement::SetPos(const float &x, const float &y) {
	pos[0] = x;
	pos[1] = y;
}

float GUIElement::GetPos(float *x, float *y) const {
	if (x)
		*x = pos[0];
	if (y)
		*y = pos[1];
}

float GUIElement::GetPosX() const {
	return pos[0];
}

float GUIElement::GetPosY() const {
	return pos[1];
}

void GUIElement::SetSize(const float &width, const float &height) {
	size[0] = width;
	size[1] = height;
}

float GUIElement::GetSize(float *width, float *height) const {
	if (width)
		*width = size[0];
	if (height)
		*height = size[1];
}

float GUIElement::GetSizeX() const {
	return size[0];
}

float GUIElement::GetSizeY() const {
	return size[1];
}

void GUIElement::SetAlign(const byte &x, const byte &y) {
	align[0] = x;
	align[1] = y;
}

byte GUIElement::GetAlign(byte *x, byte *y) const {
	if (x)
		*x = align[0];
	if (y)
		*y = align[1];
}

byte GUIElement::GetAlignX() const {
	return align[0];
}

byte GUIElement::GetAlignY() const {
	return align[1];
}

void GUIElement::SetEnabled(const byte v) {
	enabled = v;
}

byte GUIElement::GetEnabled() const {
	return enabled;
}

void GUIElement::SetType(const byte t) {
	type = t;
}

byte GUIElement::GetType() const {
	return type;
}

void GUIElement::SetParent(GUIElement *elem) {
	parent = elem;
}

GUIElement *GUIElement::GetParent() const {
	return parent;
}

void GUIElement::AddChild(GUIElement *elem) {
	if (elem != NULL)
		childs->push_back(elem);
}

void GUIElement::RemoveChild(GUIElement *elem) {
	if (elem != NULL)
		childs->remove(elem);
}

std::list<GUIElement *> *GUIElement::GetChilds() const {
	return childs;
}

void GUIElement::Render() const {

}

GUIElementText::GUIElementText() {
	text = new std::string();
}

~GUIElementText::GUIElementText() {
	delete text;
}

void GUIElementText::SetText(std::string &t) {
	*text = t;
}

std::string *GUIElementText::GetText() const {
	return text;
}
