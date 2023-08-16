#pragma once
#include "drawable.h"

class Line : public Drawable
{
public:
	D2D1_POINT_2F pointA;
	D2D1_POINT_2F pointB;
	float thickness;

	Line(Control* parent, Point pointA, Point pointB);
	void setPoints(Point pointA, Point pointB);
	void setThickness(float thickness);
	void draw() override;
};