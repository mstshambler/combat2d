#include "gui.h"

GUIElementTableItemText::GUIElementTableItemText(Texturer *texturer, Render *renderer) : GUIElementText(texturer, renderer) {
	type = GUIElement::GUIElementType_TableItemText;
}

GUIElementTableItemText::GUIElementTableItemText(Texturer *texturer, Render *renderer, const wstring &id, const wstring &text, const int &textSize, const int &sizeX, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementText(texturer, renderer, id, text, textSize, 0, 0, sizeX, 0, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_TableItemText;
}

GUIElementTableItemText::~GUIElementTableItemText() {}







GUIElementTableItemImage::GUIElementTableItemImage(Texturer *texturer, Render *renderer) : GUIElementTableItemText(texturer, renderer) {
	type = GUIElement::GUIElementType_TableItemImage;
	tex = 0;
	texCoords.Set(0.0f, 0.0f);
	texSize.Set(0.0f, 0.0f);
}

GUIElementTableItemImage::GUIElementTableItemImage(Texturer *texturer, Render *renderer, const wstring &id, const GLuint &tex, const float &tx, const float &ty, const float &tsx, const float &tsy,
	const int &sizeX, const byte &measureType, const byte &align, const byte &enabled, GUIElement *parent) : GUIElementTableItemText(texturer, renderer, id, L"", Render::FontSize_Tiny, sizeX,
	measureType, align, enabled, parent) {
	this->tex = tex;
	texCoords.Set(tx, ty);
	texSize.Set(tsx, tsy);
}

GUIElementTableItemImage::~GUIElementTableItemImage() {}

void GUIElementTableItemImage::SetTex(const GLuint &t) {
	tex = t;
}

GLuint GUIElementTableItemImage::GetTex() const {
	return tex;
}

PointFloat *GUIElementTableItemImage::TexCoords() {
	return &texCoords;
}
PointFloat *GUIElementTableItemImage::TexSize() {
	return &texSize;
}







GUIElementTableRow::GUIElementTableRow(Texturer *texturer, Render *renderer) : GUIElement(texturer, renderer) {
	type = GUIElement::GUIElementType_TableRow;
}

GUIElementTableRow::GUIElementTableRow(Texturer *texturer, Render *renderer, const wstring &id) : GUIElement(texturer, renderer, id, 0, 0, 0, 0, 0, 0, 1, GUIElement::GUIElementType_TableRow, NULL) {
}

GUIElementTableRow::~GUIElementTableRow() {}






GUIElementTable::GUIElementTable(Texturer *texturer, Render *renderer) : GUIElementMultilineText(texturer, renderer) {
	type = GUIElement::GUIElementType_Table;
	rows = new list<GUIElementTableRow *>();
	header = new GUIElementTableRow(texturer, renderer);
}

GUIElementTable::GUIElementTable(Texturer *texturer, Render *renderer, const wstring &id, const int &x, const int &y, const int &sizeX, const int &sizeY, const byte &measureType,
	const byte &align, const byte &enabled, GUIElement *parent) : GUIElementMultilineText(texturer, renderer, id, L"", Render::FontSize_Tiny, x, y, sizeX, sizeY, measureType, align, enabled, parent) {
	type = GUIElement::GUIElementType_Table;
	rows = new list<GUIElementTableRow *>();
	header = new GUIElementTableRow(texturer, renderer);
}

GUIElementTable::~GUIElementTable() {
	list<GUIElementTableRow *>::iterator ri;

	for (ri = rows->begin(); ri != rows->end(); ri++) {
		delete *ri;
	}

	delete rows;
	delete header;
}

void GUIElementTable::RenderElement(const byte &active, const byte &hover) const {}

void GUIElementTable::UpdateSize() {}

void GUIElementTable::AddRow(GUIElementTableRow *row) {
	if (row)
		rows->push_back(row);
}

void GUIElementTable::DeleteRow(GUIElementTableRow *row) {
	if (row)
		rows->remove(row);
}

list<GUIElementTableRow *> *GUIElementTable::GetRows() {
	return rows;
}

GUIElementTableRow *GUIElementTable::GetHeader() {
	return header;
}
