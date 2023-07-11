#pragma once
#include "rectangle.h"

class Label : public Rect
{
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;
	DWRITE_TEXT_ALIGNMENT textAlignment;
	IDWriteTextFormat* textFormat;
public:
	LPCWSTR text;

	Label(Control* parent, float x, float y, float width, float height);
	void setText(std::string const& text);
	void setTextAlignment(DWRITE_TEXT_ALIGNMENT alignment);
	void setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment);
	void setTextFormat(std::string const& fontFamily, float fontSize = 14.0f, DWRITE_FONT_WEIGHT fontWeight = FontWeightRegular, DWRITE_FONT_STYLE fontStyle = FontStyleNormal);
	void update() override;
	void draw() override;
};