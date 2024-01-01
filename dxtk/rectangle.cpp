#include "rectangle.h"

Rect::Rect(Control* parent, float x, float y, float width, float height)
: Drawable(parent), rounded(false), radius(0.0f), borderWidth(0.0f), borderColor(Color(Color::Black))
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
	resource.renderTarget->CreateSolidColorBrush(borderColor, &borderBrush);

	registerSignal(this, "update_rect", [this](){
		this->rect.right = this->width;
		this->rect.bottom = this->height;
		this->rRect.rect = this->rect;
	});
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

void Rect::setBorderWidth(float borderWidth)
{
	this->borderWidth = borderWidth;
	requestRedraw();
}

void Rect::setBorderColor(ColorType color)
{
	this->borderColor = color;
	borderBrush->SetColor(color);
	requestRedraw();
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
	invokeSignal("update_rect");
	requestRedraw();
}

void Rect::setWidth(float width)
{
	Control::setWidth(width);
	invokeSignal("update_rect");
	requestRedraw();
}

void Rect::setHeight(float height)
{
	Control::setHeight(height);
	invokeSignal("update_rect");
	requestRedraw();
}

void Rect::draw()
{
	pushRectClip();

	if (rounded)
	{
		resource.renderTarget->FillRoundedRectangle(rRect, brush);
		if (borderWidth > 0.0f)
			resource.renderTarget->DrawRoundedRectangle(rRect, borderBrush, borderWidth);
	}
	else
	{
		resource.renderTarget->FillRectangle(rect, brush);
		if (borderWidth > 0.0f)
			resource.renderTarget->DrawRectangle(rect, borderBrush, borderWidth);
	}

	popRectClip();
}