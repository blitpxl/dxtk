#include "line.h"

Line::Line(Control* parent, Point pointA, Point pointB)
: Control(parent), pointA(D2D1::Point2F(pointA.x, pointA.y)), pointB(D2D1::Point2F(pointB.x, pointB.y)), thickness(1.0f)
{
	is_drawable = true;
	visible = true;
	resource.renderTarget->CreateSolidColorBrush(Color(Color::White), &brush);
}

void Line::setPoints(Point pointA, Point pointB)
{
	this->pointA.x = pointA.x;
	this->pointA.y = pointA.y;
	this->pointB.x = pointB.x;
	this->pointB.y = pointB.y;
	requestRedraw();
}

void Line::setColor(ColorType color)
{
	brush->SetColor(color);
	requestRedraw();
}

void Line::setThickness(float thickness)
{
	this->thickness = thickness;
	requestRedraw();
}

void Line::update()
{
	Control::update();
	if (visible)
	{
		D2D1::Matrix3x2F transformTrans = D2D1::Matrix3x2F::Translation(x, y);
		resource.renderTarget->SetTransform(transformTrans);
	}
}

void Line::draw()
{
	resource.renderTarget->DrawLine(pointA, pointB, brush, thickness);
}