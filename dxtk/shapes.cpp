#include "shapes.h"
#include "window.h"

Shapes::Shapes(Window* window)
{
	renderer = &window->renderer;
	renderer->renderTarget->CreateSolidColorBrush(Color(Color::White), &brush);
}

void Shapes::DrawRect(float x, float y, float width, float height, ColorType color)
{
	RectFType rect = RectF(0, 0, width, height);
	brush->SetColor(color);
	renderer->renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(x, y));
	renderer->renderTarget->FillRectangle(rect, brush);
}

void Shapes::DrawEllipse(float x, float y, float size, ColorType color)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), size, size);
	brush->SetColor(color);
	renderer->renderTarget->FillEllipse(ellipse, brush);
}