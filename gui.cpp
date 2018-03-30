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
		renderer->DrawRect(px, y, size, size, (*c % 32) * cell, floor(*c / 32.0f) * cell, cell, cell);
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
