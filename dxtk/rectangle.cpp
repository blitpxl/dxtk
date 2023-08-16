#include "rectangle.h"

Rect::Rect(Control* parent, float x, float y, float width, float height)
: Drawable(parent), rounded(false), radius(0.0f)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	move(x, y);
	rect = RectF(0, 0, width, height);
	rRect.rect = rect;
	rRect.radiusX = 8;
	rRect.radiusY = 8;
}

void Rect::setRadius(float radius)
{
	if (radius > 0)
	{
		rounded = true;
		rRect.radiusX = radius; rRect.radiusY = radius;
		this->radius = radius;
	}
	else
	{
		radius = 0;
		rounded = false;
	}
}

void Rect::move(float x, float y)
{
	Control::move(x, y);
	requestRedraw();
}

void Rect::setX(float x)
{
	Control::setX(x);
	requestRedraw();
}

void Rect::setY(float y)
{
	Control::setY(y);
	requestRedraw();
}

void Rect::resize(float width, float height)
{
	Control::resize(width, height);
	rect.right = width;
	rect.bottom = height;
	rRect.rect = rect;
	requestRedraw();
}

void Rect::setWidth(float width)
{
	Control::setWidth(width);
	requestRedraw();
}

void Rect::setHeight(float height)
{
	Control::setHeight(height);
	requestRedraw();
}

void Rect::draw()
{
	pushRectClip();

	if (rounded)
		resource.renderTarget->FillRoundedRectangle(rRect, brush);
	else
		resource.renderTarget->FillRectangle(rect, brush);

	popRectClip();
}