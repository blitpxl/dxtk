#include <rectangle.h>
#include <shitlog.h>

Rect::Rect(Control* parent, float x, float y, float width, float height)
: Control(parent)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	move(x, y);
	rect = D2D1::RectF(0, 0, width, height);
	resource.renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
}

void Rect::setColor(D2D1_COLOR_F color)
{
	brush->SetColor(color);
}

void Rect::update()
{
	Control::update();
	resource.renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	resource.renderTarget->FillRectangle(rect, brush);
}