#pragma once
#include <rectangle.h>

class Label : public Rect
{
	IDWriteTextFormat* textFormat;
public:
	LPCWSTR text;

	Label(Control* parent, float x, float y, float width, float height);
	void setText(LPCWSTR text);
	void setTextAlignment(DWRITE_TEXT_ALIGNMENT alignment);
	void setParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT alignment);
	void update() override;
};