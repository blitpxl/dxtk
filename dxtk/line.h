#pragma once
#include "control.h"

class Line : public Control
{
	ID2D1SolidColorBrush* brush;
public:
	D2D1_POINT_2F pointA;
	D2D1_POINT_2F pointB;
	float thickness;

	Line(Control* parent, Point pointA, Point pointB);
	void setPoints(Point pointA, Point pointB);
	void setColor(ColorType color);
	void setThickness(float thickness);
	void update() override;
	void draw() override;
};