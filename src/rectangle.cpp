#include <rectangle.h>
#include <shitlog.h>
#include <window.h>

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

void Rect::move(float x, float y)
{
	Control::move(x, y);
	requestRedraw();
}

void Rect::resize(float width, float height)
{
	Control::resize(width, height);
	rect.right = width;
	rect.bottom = height;
	requestRedraw();
}

void Rect::setColor(D2D1_COLOR_F color)
{
	brush->SetColor(color);
	requestRedraw();
}

void Rect::update()
{
	Control::update();
	resource.renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	resource.renderTarget->FillRectangle(rect, brush);
}