#pragma once
#include "rectangle.h"

class Label : public Rect
{
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;
	DWRITE_TEXT_ALIGNMENT textAlignment;
public:
	IDWriteTextFormat* textFormat;
	IDWriteTextLayout* textLayout;
	LPCWSTR text;
	float scale;
	float caretX;

	Label(Control* parent, float x, float y, float width, float height);
	void setScale(float scale);
	void setText(std::string const& text);
	void setTextAlignment(DWRITE_TEXT_ALIGNMENT alignment);
	void setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment);
	void setTextFormat(std::string const& fontFamily, float fontSize = 14.0f, DWRITE_FONT_WEIGHT fontWeight = FontWeightRegular, DWRITE_FONT_STYLE fontStyle = FontStyleNormal);
	FontMetrics getFontMetrics(UINT32 textPosition);
	void update() override;
	void draw() override;
};