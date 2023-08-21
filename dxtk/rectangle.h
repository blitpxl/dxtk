#pragma once
#include "drawable.h"

class Rect : public Drawable
{
	bool rounded;
	RRect rRect;
	ID2D1SolidColorBrush* borderBrush;
public:
	ColorType borderColor;
	float borderWidth;
	float radius;

	Rect(Control* parent, float x, float y, float width, float height);
	void setRadius(float radius);
	void setBorderWidth(float borderWidth);
	void setBorderColor(ColorType color);
	void move(float x, float y) override;
	void setX(float x) override;
	void setY(float y) override;
	void resize(float width, float height) override;
	void setWidth(float width) override;
	void setHeight(float height) override;
	void draw() override;
};