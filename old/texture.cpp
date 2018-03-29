#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef WIN32
#include <GL/glew.h>
#endif

#include <GL/glut.h>

#include <string.h>
#include <time.h>

#include "common.h"
#include "texture.h"

Texture::Texture() {
	image_width=0;
	image_height=0;
	image_pixelsize=0;
	texnum_current = 0;

	texnum_max = 1;
}

Texture::~Texture() {
}

int Texture::fgetLittleShort (FILE *f) {
	byte b1, b2;

	b1 = fgetc(f);
	b2 = fgetc(f);

	return (short)(b1 + b2*256);
}

int Texture::fgetLittleLong (FILE *f) {
	byte b1, b2, b3, b4;

	b1 = fgetc(f);
	b2 = fgetc(f);
	b3 = fgetc(f);
	b4 = fgetc(f);

	return b1 + (b2<<8) + (b3<<16) + (b4<<24);
}

byte* Texture::LoadTGA (FILE *fin) {
	int columns, rows;
	long numPixels;
	byte *image_rgba;

	targa_header.id_length = fgetc(fin);
	targa_header.colormap_type = fgetc(fin);
	targa_header.image_type = fgetc(fin);

	targa_header.colormap_index = fgetLittleShort(fin);
	targa_header.colormap_length = fgetLittleShort(fin);
	targa_header.colormap_size = fgetc(fin);
	targa_header.x_origin = fgetLittleShort(fin);
	targa_header.y_origin = fgetLittleShort(fin);
	targa_header.width = fgetLittleShort(fin);
	targa_header.height = fgetLittleShort(fin);
	targa_header.pixel_size = fgetc(fin);
	targa_header.attributes = fgetc(fin);

	if (targa_header.image_type!=2
		&& targa_header.image_type!=10)
		return NULL;

	if (targa_header.colormap_type !=0
		|| (targa_header.pixel_size!=32 && targa_header.pixel_size!=24))
		return NULL;

	// Uncompressed RGB/RGBA only
	if (targa_header.image_type != 2)
		return NULL;

	columns = targa_header.width;
	rows = targa_header.height;
	numPixels = columns * rows;

	if (targa_header.pixel_size == 24)
		image_rgba = (unsigned char *)malloc (numPixels*3);
	else 
		image_rgba = (unsigned char *)malloc (numPixels*4);

	if (targa_header.id_length != 0)
		fseek(fin, targa_header.id_length, SEEK_CUR);  // skip TARGA image comment

	fread(image_rgba, 1, rows*columns*(targa_header.pixel_size == 24 ? 3 : 4), fin);

	image_width = columns;
	image_height = rows;
	image_pixelsize = targa_header.pixel_size;

	return image_rgba;
}


void Texture::Bind(int texnum) {
	if (texnum_current == texnum)
		return;

	texnum_current = texnum;
	glBindTexture(GL_TEXTURE_2D, texnum);
}

int Texture::LoadTexture(char *name) {
	FILE *fp;
	byte *bp;

	fp = fopen(name,"rb");
	if (fp) {
		bp = LoadTGA(fp);

		Bind(texnum_max);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		texnum_max++;

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if (image_pixelsize == 24)
			glTexImage2D(GL_TEXTURE_2D, 0, 3, this->image_width, this->image_height, 0, GL_BGR, GL_UNSIGNED_BYTE, bp);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, 4, this->image_width, this->image_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bp);

		fclose(fp);
		return texnum_max-1;
	}
	return 0;
}

void Texture::LoadTexturesList(void) {
	texnum_gfx = LoadTexture("gfx.tga");
	texnum_font = LoadTexture("dialogfont32.tga");
	texnum_fontFixed = LoadTexture("font16.tga");
}

int Texture::GetGFX() {
	return texnum_gfx;
}

int Texture::GetFont() {
	return texnum_font;
}

int Texture::GetFontFixed() {
	return texnum_fontFixed;
}