class Render {
protected:
	int screen_width;
	int screen_height;

public:
	Render();
	~Render();

	void Init(int width, int height);
	void SetTextureMode();
	void Begin(int width = -1, int height = -1);
	void End();
	void Set2DMode(int width = -1, int height = -1);
	void End2DMode();
	void Shutdown();
	void Clear();

	int GetScreenWidth();
	int GetScreenHeight();
	void Reshape(int width, int height);

	void DrawSprite(const float &x, const float &y, const float &width, const float &height, const float &tx, const float &ty, const float &t_width, const float &t_height, const float &angle);
};

extern Render *render;
