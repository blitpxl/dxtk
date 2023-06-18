#pragma once
#include <control.h>

class Rect : public Control
{
protected:
	D2D1_RECT_F rect;
	ID2D1SolidColorBrush* brush;

public:
	Rect(Control* parent, float x, float y, float width, float height);
	void setColor(D2D1_COLOR_F color);
	void update() override;
};