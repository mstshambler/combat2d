#define _USE_MATH_DEFINES

#include "render.h"

RenderFontCharacter::RenderFontCharacter(const int &pos_x, const int &pos_y, const int &bearing_x, const int &bearing_y, const GLuint &shift_x, const GLuint &shift_y) {
	pos.Set(pos_x, pos_y);
	bearing.Set(bearing_x, bearing_y);
	shift[0] = shift_x;
	shift[1] = shift_y;
}

RenderFontCharacter::~RenderFontCharacter() {
}

PointInt *RenderFontCharacter::Pos() {
	return &pos;
}

PointInt *RenderFontCharacter::Bearing() {
	return &bearing;
}

GLuint RenderFontCharacter::GetShiftX() const {
	return shift[0];
}

GLuint RenderFontCharacter::GetShiftY() const {
	return shift[1];
}






RenderFont::RenderFont(const wstring &name, const int &size, const int &pixelSize, FT_Library &ftLibrary, list<GLuint> &usableChars) {
	FT_Face ftFace;
	string pathToFont;
	list<GLuint>::iterator uci;

	uint i;
	int charsCount = 0;
	int charsX = 0;
	int charsY = 0;
	int charPosX = 0;
	int charPosY = 0;
	byte *texBuffer;

	title = new wstring();
	*title = name;
	this->size = size;
	this->pixelSize = pixelSize;

	pathToFont = "data/fonts/";
	{
		char buf[256];

		wcstombs(buf, name.c_str(), 250);
		buf[250] = 0;
		pathToFont.append(buf);
	}

	if (FT_New_Face(ftLibrary, pathToFont.c_str(), 0, &ftFace))
		printf("FreeType: face init error");

	FT_Set_Pixel_Sizes(ftFace, 0, pixelSize);
	//FT_Set_Pixel_Sizes(ftFace, pixelSize, 0);

	for (uci = usableChars.begin(); uci != usableChars.end(); uci++) {
		if (FT_Load_Char(ftFace, *uci, FT_LOAD_RENDER))
			continue;

		if (charSize.GetX() < ftFace->glyph->bitmap.width)
			charSize.SetX(ftFace->glyph->bitmap.width);
		if (charSize.GetY() < ftFace->glyph->bitmap.rows)
			charSize.SetY(ftFace->glyph->bitmap.rows);
		charsCount++;
	}

	// get initial texture width;
	charsX = (int)sqrt(charsCount);
	charsY = charsX;
	if (charsX * charsY < charsCount)
		charsX++;
	texSize.SetX(charsX * charSize.GetX());

	// make it power of 2
	i = 1;
	while (i < texSize.GetX())
		i *= 2;
	texSize.SetX(i);

	// get texture height
	charsX = (int)floor(texSize.GetX() / charSize.GetX());
	charsY = (int)ceil(charsCount / charsY);
	texSize.SetY(charSize.GetY() * charsY);

	// make it power of 2 too
	i = 1;
	while (i < texSize.GetY())
		i *= 2;
	texSize.SetY(i);

	texBuffer = new byte[texSize.GetX() * texSize.GetY()];
	memset(texBuffer, 0, sizeof(byte)* texSize.GetX() * texSize.GetY());

	for (uci = usableChars.begin(); uci != usableChars.end(); uci++) {
		RenderFontCharacter *fontChar;
		uint x, y, z;

		if (FT_Load_Char(ftFace, *uci, FT_LOAD_RENDER))
			continue;

		fontChar = new RenderFontCharacter(charPosX * charSize.GetX(), charPosY * charSize.GetY(), ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top,
			ftFace->glyph->advance.x >> 6, ftFace->glyph->advance.y >> 6);
		chars.insert(pair<GLuint, RenderFontCharacter *>(*uci, fontChar));

		for (y = 0, z = 0; y < ftFace->glyph->bitmap.rows; y++)
		for (x = 0; x < ftFace->glyph->bitmap.width; x++, z++)
			texBuffer[charPosY * charSize.GetY() * texSize.GetX() + y * texSize.GetX() + charPosX * charSize.GetX() + x] = ftFace->glyph->bitmap.buffer[z];

		charPosX++;
		if (charPosX >= charsX) {
			charPosY++;
			charPosX = 0;
		}
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, texSize.GetX(), texSize.GetY(), 0, GL_ALPHA, GL_UNSIGNED_BYTE, texBuffer);

	delete texBuffer;
	FT_Done_Face(ftFace);
}

RenderFont::~RenderFont() {
	delete title;
}

RenderFontCharacter *RenderFont::GetCharacterInfo(const GLuint &charId) {
	map<GLuint, RenderFontCharacter *>::iterator ci;

	ci = chars.find(charId);
	if (ci != chars.end())
		return ci->second;
	return NULL;
}

wstring *RenderFont::GetTitle() const {
	return title;
}

int RenderFont::GetSize() const {
	return size;
}

int RenderFont::GetPixelSize() const {
	return pixelSize;
}

GLuint RenderFont::GetTexture() const {
	return texId;
}

PointUInt *RenderFont::TextureSize() {
	return &texSize;
}

PointUInt *RenderFont::CharacterSize() {
	return &charSize;
}







Render::Render() {
	FILE *fp;
	int i;

	screenWidth = 0;
	screenHeight = 0;

	tickMsecs = -1.0f;
	totalTime.tv_sec = 0;
	totalTime.tv_usec = 0;

	// load available chars
	for (i = 0; i < 256; i++)
		usableChars.push_back(i);

	fp = _wfopen(L"data/chars.cfg", L"rt+, ccs=UTF-8");
	if (fp) {
		wchar_t buf[24];

		while (fgetws(buf, 16, fp)) {
			usableChars.push_back(buf[0]);
		}

		fclose(fp);
	}

	fonts = new list<RenderFont *>();
}

Render::~Render() {
	list<RenderFont *>::iterator fi;

	if (fonts) {
		for (fi = fonts->begin(); fi != fonts->end(); fi++)
			delete *fi;

		fonts->clear();
		delete fonts;
	}
}

int Render::GetScreenWidth() const {
	return screenWidth;
}

int Render::GetScreenHeight() const {
	return screenHeight;
}

void Render::Reshape(const int &width, const int &height) {
	screenWidth = width;
	screenHeight = height;
}

void Render::Init(const int &width, const int &height) {
	int		x, x2, y2, y;
	double screenaspect;

	// define screen resolution
	screenWidth = width;
	screenHeight = height;
	screenWidth2D = width;
	screenHeight2D = height;

	x = 0;
	x2 = width;
	y = height;
	y2 = 0;

	// Set default OpenGL settings
	glClearColor( 0.0F, 0.0F, 0.0F, 0.0F );
	glClearStencil(0);
	glShadeModel( GL_SMOOTH );

	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    screenaspect = (double)width/height;
	gluPerspective (90.0f/screenaspect,  screenaspect,  0,  4096);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glViewport (x, y2, width, height);
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	glDisable(GL_LIGHT0);

//	InitFonts(1.0f);
}

void Render::Begin(int width, int height) const {
	double screenaspect;

	if (width == -1)
		width = screenWidth;
	if (height == -1)
		height = screenHeight;

	Render::Clear();

	glRenderMode(GL_RENDER);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, width, height);
    screenaspect = (double)width/height;
	gluPerspective (90.0f/screenaspect,  screenaspect,  1,  4096);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();

	glColor4f(1.0, 1.0, 1.0, 1.0);
}


void Render::SetTextureMode() const {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthRange(0, 1);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	glEnable(GL_TEXTURE_2D);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Render::End() const {
	glRenderMode(GL_RENDER);
	glDisable(GL_DEPTH_TEST);
	glDisable (GL_ALPHA_TEST);
}

void Render::Set2DMode(int width, int height, int ignoreStack) {
	if (width == -1)
		width = screenWidth;
	if (height == -1)
		height = screenHeight;

	screenWidth2D = width;
	screenHeight2D = height;

	glMatrixMode(GL_PROJECTION);
	if (!ignoreStack)
		glPushMatrix();
    glLoadIdentity();

	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	if (!ignoreStack)
		glPushMatrix();
    glLoadIdentity ();

	//glDisable (GL_DEPTH_TEST);
	glDisable (GL_CULL_FACE);
	glEnable (GL_ALPHA_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f (1,1,1,1);
}

void Render::End2DMode(int ignoreStack) const {
	if (!ignoreStack) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

void Render::Shutdown(void) const {
}

void Render::Clear (void) const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render::DrawTexturedRect(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height) const {
	DrawSprite(x + width / 2, y - height / 2, width, height, tx, ty, t_width, t_height, 0);
}

void Render::DrawSprite(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle) const {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(tx, ty);
	glVertex2f(-width/2.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty);
	glVertex2f(width/2.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty + t_height);
	glVertex2f(width/2.0f, -height/2.0f);

	glTexCoord2f(tx, ty + t_height);
	glVertex2f(-width/2.0f, -height/2.0f);
	glEnd();
	glPopMatrix();
}

void Render::DrawSprite3D(const float &x, const float &y, const float &z, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(tx, ty);
	glVertex3f(-width/2.0f, 0.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty);
	glVertex3f(width/2.0f, 0.0f, height/2.0f);

	glTexCoord2f(tx + t_width, ty + t_height);
	glVertex3f(width/2.0f, 0.0f, -height/2.0f);

	glTexCoord2f(tx, ty + t_height);
	glVertex3f(-width/2.0f, 0.0f, -height/2.0f);
	glEnd();
	glPopMatrix();
}

void Render::ConvertWindowToScreenCoords(const float &x, const float &y, float &sx, float &sy) const {
	sx = x * screenWidth2D / screenWidth;
	sy = screenHeight2D - 1.0f - y * screenHeight2D / screenHeight;
}

void Render::DrawBorder(const float &x, const float &y, const float &width, const float &height, const float &size, const float &r, const float &g, const float &b, const float &a) const {
	glLineWidth(size);
	glColor4f(r, g, b, a);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - height);
	glVertex2f(x, y - height);
	glEnd();
	glLineWidth(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Render::DrawRect(const float &x, const float &y, const float &width, const float &height, const float &r, const float &g, const float &b, const float &a) const {
	glColor4f(r, g, b, a);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - height);
	glVertex2f(x, y - height);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Render::DrawCircle(const float &x, const float &y, const float &radius, const byte &filled, const float &r, const float &g, const float &b, const float &a) const {
	float cur;
	float step;

	glColor4f(r, g, b, a);
	if (filled)
		glBegin(GL_TRIANGLE_FAN);
	else
		glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.0f, 0.0f);
	if (filled)
		glVertex2f(x, y);

	cur = 0.0f;
	step = (float)M_PI * 2.0f / 360.0f;
	while (cur < (float)M_PI * 2.0f) {
		glVertex2f(x + cos(cur) * radius, y + sin(cur) * radius);
		cur += step;
	}

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Render::ReadCoordsUnderCursor(const int &x, const int &y, float *ox, float *oy, float *oz) {
	GLfloat wz;
	GLint wy;
	GLdouble objpos[3];
	GLdouble model_view[16];
	GLdouble projection[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glReadBuffer(GL_FRONT);
	wy = viewport[3] - y;
	glReadPixels(x, wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	gluUnProject(x, wy, wz,
		model_view, projection, viewport,
		&objpos[0], &objpos[1], &objpos[2]);

	objpos[1] *= -1;
	glReadBuffer(GL_BACK);

	if (ox)
		*ox = (float)objpos[0];
	if (oy)
		*oy = (float)objpos[1];
	if (oz)
		*oz = (float)objpos[2];
}

void Render::InitFonts(const float &zoom) {
	FT_Library ftLibrary;
	RenderFont *font;
	list<RenderFont *>::iterator fi;
	float fontScale;

	fontScale = (float)(screenHeight / 1080.0f) * zoom;

	if (FT_Init_FreeType(&ftLibrary)) {
		printf("FreeType: init error");
		return;
	}

	for (fi = fonts->begin(); fi != fonts->end(); fi++)
		delete *fi;

	fonts->clear();
	font = new RenderFont(L"arial.ttf", FontSize::FontSize_Tiny, (int)(16.0f * fontScale), ftLibrary, usableChars);
	fonts->push_back(font);
	font = new RenderFont(L"arial.ttf", FontSize::FontSize_Small, (int)(18.0f * fontScale), ftLibrary, usableChars);
	fonts->push_back(font);
	font = new RenderFont(L"arial.ttf", FontSize::FontSize_Medium, (int)(22.0f * fontScale), ftLibrary, usableChars);
	fonts->push_back(font);
	font = new RenderFont(L"arial.ttf", FontSize::FontSize_Large, (int)(32.0f * fontScale), ftLibrary, usableChars);
	fonts->push_back(font);
	font = new RenderFont(L"arial.ttf", FontSize::FontSize_Huge, (int)(48.0f * fontScale), ftLibrary, usableChars);
	fonts->push_back(font);

	FT_Done_FreeType(ftLibrary);
}

RenderFont *Render::FindFont(const int &size, const wstring &fontName) const {
	RenderFont *font;
	list<RenderFont *>::iterator fontI;

	font = NULL;
	fontI = fonts->begin();
	while (fontI != fonts->end()) {
		font = *fontI;
		if (*font->GetTitle() == fontName && font->GetSize() == size) {
			break;
		}
		fontI++;
	}
	return font;
}

void Render::DrawString(Texturer *texturer, const float &x, const float &y, const int &size, const wstring &fontName, const wstring &text) const {
	RenderFont *font;

	font = FindFont(size, fontName);
	if (font) {
		wchar_t *c;
		float px;

		px = x;
		c = (wchar_t *)text.c_str();

		texturer->Bind(font->GetTexture());

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		while (*c) {
			RenderFontCharacter *charInfo;

			charInfo = font->GetCharacterInfo(*c);
			if (charInfo) {
				if (*c != L'\n' && *c != L'\r') {
					float tx, ty, sx, sy;

					tx = (float)charInfo->Pos()->GetX() / (float)font->TextureSize()->GetX();
					ty = (float)charInfo->Pos()->GetY() / (float)font->TextureSize()->GetY();
					sx = (float)font->CharacterSize()->GetX() / (float)font->TextureSize()->GetX();
					sy = (float)font->CharacterSize()->GetY() / (float)font->TextureSize()->GetY();

					DrawTexturedRect(px + (float)charInfo->Bearing()->GetX(), y - (float)font->GetPixelSize() + (float)charInfo->Bearing()->GetY(),
						(float)font->CharacterSize()->GetX(), (float)font->CharacterSize()->GetY(), tx, ty, sx, sy);
					px += (float)charInfo->GetShiftX();
				}
			}
			c++;
		}
	}
}

void Render::DrawStringBox(Texturer *texturer, const float &boxX, const float &boxY, const float &boxSizeX, const float &boxSizeY,
	const byte &horizontalWrap, const float &x, const float &y, const int &size, const wstring &fontName, const wstring &text) const {
	RenderFont *font;

	font = FindFont(size, fontName);
	if (font) {
		wchar_t *c;
		float cx;
		float cy;

		cx = x;
		cy = y;
		c = (wchar_t *)text.c_str();

		texturer->Bind(font->GetTexture());

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		while (*c) {
			RenderFontCharacter *charInfo;

			charInfo = font->GetCharacterInfo(*c);
			if (charInfo) {
				float tx, ty, tsx, tsy, px, py, sx, sy;

				if (*c != L'\n' && *c != L'\r') {
					if (horizontalWrap && cx + (float)charInfo->GetShiftX() > boxX + boxSizeX) {
						cx = x;
						cy -= (float)font->GetPixelSize();
					}

					px = cx + (float)charInfo->Bearing()->GetX();
					py = cy - (float)font->GetPixelSize() + (float)charInfo->Bearing()->GetY();
					sx = (float)font->CharacterSize()->GetX();
					sy = (float)font->CharacterSize()->GetY();

					if (boxY > py - sy && boxY - boxSizeY < py && boxX < px + sx && boxX + boxSizeX > px) {
						float txp, typ;

						if (boxX + boxSizeX < px + sx)
							sx = boxX + boxSizeX - px;

						if (boxY - boxSizeY > py - sy)
							sy = py - (boxY - boxSizeY);

						txp = (float)charInfo->Pos()->GetX();
						if (boxX > px) {
							txp += boxX - px;
							sx -= boxX - px;
							px = boxX;
						}

						typ = (float)charInfo->Pos()->GetY();
						if (boxY < py) {
							typ += py - boxY;
							sy -= py - boxY;
							py = boxY;
						}

						tx = txp / (float)font->TextureSize()->GetX();
						ty = typ / (float)font->TextureSize()->GetY();
						tsx = sx / (float)font->TextureSize()->GetX();
						tsy = sy / (float)font->TextureSize()->GetY();

						DrawTexturedRect(px, py, sx, sy, tx, ty, tsx, tsy);
					}
					cx += (float)charInfo->GetShiftX();
				} else if (*c == L'\n') {
					cx = x;
					cy -= (float)font->GetPixelSize();
				}
			}
			c++;
		}
	}
}

float Render::GetStringLength(const int &size, const wstring &fontName, const wstring &text, const int &len) const {
	RenderFont *font;
	float px;

	px = 0.0f;

	font = FindFont(size, fontName);
	if (font) {
		wchar_t *c;
		int k = 0;

		c = (wchar_t *)text.c_str();
		while (*c && (len < 0 || k < len)) {
			RenderFontCharacter *charInfo;

			charInfo = font->GetCharacterInfo(*c);
			if (charInfo)
				px += (float)charInfo->GetShiftX();
			c++;
			k++;
		}
	}
	return px;
}

float Render::GetStringHeight(const int &size, const wstring &fontName, const wstring &text, const float &boxSizeX) const {
	RenderFont *font;
	float cy;

	font = FindFont(size, fontName);
	if (font) {
		wchar_t *c;
		float cx;

		cx = 0.0f;
		cy = 0.0f;
		c = (wchar_t *)text.c_str();

		while (*c) {
			RenderFontCharacter *charInfo;

			charInfo = font->GetCharacterInfo(*c);
			if (charInfo) {
				if (*c != L'\n' && *c != L'\r') {
					if (boxSizeX > 0.0f && cx + (float)charInfo->GetShiftX() > boxSizeX) {
						cx = 0.0f;
						cy += (float)font->GetPixelSize();
					}

					cx += (float)charInfo->GetShiftX();
				} else if (*c == L'\n') {
					cx = 0.0f;
					cy += (float)font->GetPixelSize();
				}
			}
			c++;
		}
	}
	return cy + (float)font->GetPixelSize() * 1.5f;
}

void Render::SetTickMsecs(const float &f) {
	tickMsecs = f;
}

float Render::GetTickMSecs() const {
	return tickMsecs;
}

void Render::SetTotalTime(const struct timeval &t) {
	totalTime.tv_sec = t.tv_sec;
	totalTime.tv_usec = t.tv_usec;
}

struct timeval *Render::GetTickTime() {
	return &totalTime;
}
