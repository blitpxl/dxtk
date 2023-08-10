#include "rectangle.h"
#include "window.h"

Rect::Rect(Control* parent, float x, float y, float width, float height)
: Control(parent), rounded(false), scale(1.0f), radius(0.0f), color(Color(Color::White)), clipped(false), clip_pushed(false)
{
	is_drawable = true;
	visible = true;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	move(x, y);
	rect = RectF(0, 0, width, height);
	rRect.rect = rect;
	rRect.radiusX = 8;
	rRect.radiusY = 8;
	resource.renderTarget->CreateSolidColorBrush(color, &brush);
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

void Rect::setScale(float scale)
{
	this->scale = scale;
	requestRedraw();
}

void Rect::setColor(ColorType color)
{
	brush->SetColor(color);
	this->color = color;
	requestRedraw();
}

void Rect::addClipSource(Control* control)
{
	clipSources.push_back(control);
}

void Rect::update()
{
	Control::update();
	if (visible)
	{
		D2D1::Matrix3x2F transformScale = D2D1::Matrix3x2F::Scale(D2D1::Size(scale, scale), D2D1::Point2F(width/2, height/2));
		D2D1::Matrix3x2F transformTrans = D2D1::Matrix3x2F::Translation(x, y);
		resource.renderTarget->SetTransform(transformScale * transformTrans);
		resource.renderTarget->GetTransform(&transform);
	}
}

void Rect::draw()
{
	resource.window->saveRenderState();
	for (Control* clipSource : clipSources)
	{
		Rect* source = static_cast<Rect*>(clipSource);
		resource.renderTarget->SetTransform(source->transform);
		resource.renderTarget->PushAxisAlignedClip(source->rect, D2D1_ANTIALIAS_MODE_ALIASED);
	}
	resource.window->restoreRenderState();

	if (rounded)
		resource.renderTarget->FillRoundedRectangle(rRect, brush);
	else
		resource.renderTarget->FillRectangle(rect, brush);

	for (int i = 0; i < clipSources.size(); i++)
	{
		resource.renderTarget->PopAxisAlignedClip();
	}
}