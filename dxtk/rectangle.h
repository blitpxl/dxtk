#pragma once
#include "control.h"

class Rect : public Control
{
	bool rounded;
protected:
	RRect rRect;
	ID2D1SolidColorBrush* brush;
public:
	float radius;
	ColorType color;

	Rect(Control* parent, float x, float y, float width, float height);
	void setRadius(float radius);
	void move(float x, float y) override;
	void setX(float x) override;
	void setY(float y) override;
	void resize(float width, float height) override;
	void setWidth(float width) override;
	void setHeight(float height) override;
	void setColor(ColorType color);
	virtual void update() override;
	virtual void draw() override;
};