#pragma once
#include "control.h"

class Rect : public Control
{
	bool rounded;
protected:
	D2D1_RECT_F rect;
	D2D1_ROUNDED_RECT rRect;
	ID2D1SolidColorBrush* brush;
public:
	float radius;
	D2D1_COLOR_F color;

	Rect(Control* parent, float x, float y, float width, float height);
	void setRadius(float radius);
	void move(float x, float y) override;
	void setX(float x) override;
	void setY(float y) override;
	void resize(float width, float height) override;
	void setWidth(float width) override;
	void setHeight(float height) override;
	void setColor(D2D1_COLOR_F color);
	virtual void update() override;
	virtual void draw() override;
};