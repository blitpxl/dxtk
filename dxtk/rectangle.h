#pragma once
#include "control.h"

class Rect : public Control
{
	bool rounded;
	bool clipped;
	bool clip_pushed;
protected:
	RRect rRect;
	ID2D1SolidColorBrush* brush;
	ID2D1SolidColorBrush* borderBrush;
	std::vector<Control*> clipSources;
public:
	RectFType rect;
	D2D1_MATRIX_3X2_F transform;
	float scale;
	float radius;
	ColorType color;

	Rect(Control* parent, float x, float y, float width, float height);
	void setRadius(float radius);
	void move(float x, float y) override;
	void setX(float x) override;
	void setY(float y) override;
	void resize(float width, float height) override;
	void setWidth(float width) override;
	void setHeight(float height) override;
	void setScale(float scale);
	void setColor(ColorType color);
	void addClipSource(Control* control);
	virtual void update() override;
	virtual void draw() override;
};