class Texture {
public:
	Texture();
	~Texture();

	void LoadTexturesList(void);
	int LoadTexture(char *name);
	void Bind(int texnum);

	int GetGFX();
	int GetFont();
	int GetFontFixed();

protected:
	int texnum_max;
	int texnum_current;
	int texnum_gfx;
	int texnum_font;
	int texnum_fontFixed;

	typedef struct _TargaHeader {
		unsigned char id_length, colormap_type, image_type;
		unsigned short colormap_index, colormap_length;
		unsigned char colormap_size;
		unsigned short x_origin, y_origin, width, height;
		unsigned char pixel_size, attributes;
	} TargaHeader;

	TargaHeader targa_header;
	int image_width, image_height, image_pixelsize;

	int fgetLittleShort(FILE *f);
	int fgetLittleLong(FILE *f);
	byte *LoadTGA(FILE *fin);
};

extern Texture *texture;
