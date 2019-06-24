#ifndef _render_h
#define _render_h

#include <string>
#include <list>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "common.h"
#include "texture.h"

using namespace std;

class RenderFontCharacter {
protected:
	PointInt pos;
	PointInt bearing;
	GLuint shift[2];

public:
	RenderFontCharacter(const int &pos_x, const int &pos_y, const int &bearing_x, const int &bearing_y, const GLuint &shift_x, const GLuint &shift_y);
	~RenderFontCharacter();

	PointInt *Pos();
	PointInt *Bearing();
	GLuint GetShiftX() const;
	GLuint GetShiftY() const;
};

class RenderFont {
protected:
	wstring *title;
	map<GLuint, RenderFontCharacter *> chars;
	int size;
	int pixelSize;
	GLuint texId;
	PointUInt texSize;
	PointUInt charSize;

public:
	RenderFont(const wstring &name, const int &size, const int &pixelSize, FT_Library &ftLibrary, list<GLuint> &usableChars);
	~RenderFont();

	wstring *GetTitle() const;
	int GetSize() const;
	int GetPixelSize() const;
	RenderFontCharacter *GetCharacterInfo(const GLuint &charId);
	GLuint GetTexture() const;
	PointUInt *TextureSize();
	PointUInt *CharacterSize();
};

class Render {
protected:
	int screenWidth;
	int screenHeight;
	int screenWidth2D;
	int screenHeight2D;

	list<RenderFont *> *fonts;
	list<GLuint> usableChars;

public:
	Render();
	~Render();

	enum FontSize {
		FontSize_Tiny = 0,
		FontSize_Small = 1,
		FontSize_Medium = 2,
		FontSize_Large = 3,
		FontSize_Huge = 4
	};

	void Init(const int &width, const int &height);
	void SetTextureMode() const;
	void Begin(int width = -1, int height = -1) const;
	void End() const;
	void Set2DMode(int width = -1, int height = -1, int ignoreStack = 1);
	void End2DMode(int ignoreStack = 1) const;
	void Shutdown() const;
	void Clear() const;

	int GetScreenWidth() const;
	int GetScreenHeight() const;
	void Reshape(const int &width, const int &height);

	void DrawTexturedRect(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height) const;
	void DrawSprite(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle) const;
	void DrawBorder(const float &x, const float &y, const float &width, const float &height, const float &size, const float &r, const float &g, const float &b, const float &a) const;
	void DrawRect(const float &x, const float &y, const float &width, const float &height, const float &r, const float &g, const float &b, const float &a) const;
	void ConvertWindowToScreenCoords(const float &x, const float &y, float &sx, float &sy) const;

	void ReadCoordsUnderCursor(const int &x, const int &y, float *ox, float *oy, float *oz);
	void DrawSprite3D(const float &x, const float &y, const float &z, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle);

	void InitFonts(const float &zoom);
	RenderFont *FindFont(const int &size, const wstring &fontName) const;
	void DrawString(Texturer *texturer, const float &x, const float &y, const int &size, const wstring &fontName, const wstring &text) const;
	void DrawStringBox(Texturer *texturer, const float &boxX, const float &boxY, const float &boxSizeX, const float &boxSizeY, const float &x, const float &y, const int &size, const wstring &fontName, const wstring &text) const;
	float GetStringBoxSize(const float &boxSizeX, const float &boxSizeY, const int &size, const wstring &fontName, const wstring &text);
	float GetStringLength(const int &size, const wstring &fontName, const wstring &text) const;
	float GetStringHeight(const int &size, const wstring &fontName, const wstring &text) const;
};

#endif