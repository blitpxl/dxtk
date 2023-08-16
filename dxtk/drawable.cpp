#include "drawable.h"
#include "window.h"

Drawable::Drawable(Control* parent)
: Control(parent), scale(1.0f), rotation(0.0f), clip_pushed(false), color(Color(Color::White))
{
	is_drawable = true;
	visible = true;
	resource.renderTarget->CreateSolidColorBrush(color, &brush);
}

void Drawable::setScale(float scale)
{
	this->scale = scale;
	requestRedraw();
}

void Drawable::setRotation(float angle)
{
	rotation = angle;
	requestRedraw();
}

void Drawable::setColor(ColorType color)
{
	brush->SetColor(color);
	requestRedraw();
}

void Drawable::addClipRectSource(Control* control)
{
	clipSources.push_back(control);
}

void Drawable::update()
{
	Control::update();
	if (visible)
	{
		D2D1::Matrix3x2F transformRot = D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(width/2, height/2));
		D2D1::Matrix3x2F transformScale = D2D1::Matrix3x2F::Scale(D2D1::Size(scale, scale), D2D1::Point2F(width/2, height/2));
		D2D1::Matrix3x2F transformTrans = D2D1::Matrix3x2F::Translation(x, y);
		resource.renderTarget->SetTransform(transformRot * transformScale * transformTrans);
		resource.renderTarget->GetTransform(&transform);
	}
}

void Drawable::pushRectClip()
{
	resource.window->saveRenderState();
	for (Control* clipSource : clipSources)
	{
		Drawable* source = static_cast<Drawable*>(clipSource);
		resource.renderTarget->SetTransform(source->transform);
		resource.renderTarget->PushAxisAlignedClip(source->rect, D2D1_ANTIALIAS_MODE_ALIASED);
	}
	resource.window->restoreRenderState();
}

void Drawable::popRectClip()
{
	for (int i = 0; i < clipSources.size(); i++)
	{
		resource.renderTarget->PopAxisAlignedClip();
	}
}