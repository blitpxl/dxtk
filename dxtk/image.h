#pragma once
#include "rectangle.h"
#include <comdef.h>

class Image : public Rect
{
	IWICBitmapDecoder* decoder;
	IWICFormatConverter* formatConverter;
	IWICBitmapFrameDecode* frame;
	
public:
	ID2D1Bitmap* bitmap;

	Image(Control* parent, std::string const& imgPath, float x, float y, float width, float height);
	void setImage(std::string_view imgPath);
	void draw() override;
};