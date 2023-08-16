#pragma once
#include "control.h"

class Drawable : public Control
{
public:
	float scale;
	float rotation;
	bool clip_pushed;

	std::vector<Control*> clipSources;
	ID2D1SolidColorBrush* brush;
	D2D1_MATRIX_3X2_F transform;
	ColorType color;
	RectFType rect;

	Drawable(Control* parent);
	void setScale(float scale);
	void setRotation(float angle);
	void setColor(ColorType color);
	void addClipRectSource(Control* control);
	void update() override;

protected:
	void pushRectClip();
	void popRectClip();
};