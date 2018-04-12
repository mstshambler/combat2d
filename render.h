#ifndef _render_h
#define _render_h

#include "common.h"

class Render {
protected:
	int screenWidth;
	int screenHeight;
	int screenWidth2D;
	int screenHeight2D;

public:
	Render();
	~Render();

	void Init(const int &width, const int &height);
	void SetTextureMode() const;
	void Begin(int width = -1, int height = -1) const;
	void End() const;
	void Set2DMode(int width = -1, int height = -1);
	void End2DMode() const;
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
};

#endif