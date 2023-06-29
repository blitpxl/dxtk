#pragma once
#include <control.h>

class Rect : public Control
{
protected:
	D2D1_RECT_F rect;
	ID2D1SolidColorBrush* brush;

public:
	Rect(Control* parent, float x, float y, float width, float height);
	void move(float x, float y);
	void resize(float width, float height) override;
	void setColor(D2D1_COLOR_F color);
	virtual void update() override;
};