#pragma once
#include "drawable.h"

class Rect : public Drawable
{
	bool rounded;
	RRect rRect;
public:
	float radius;

	Rect(Control* parent, float x, float y, float width, float height);
	void setRadius(float radius);
	void move(float x, float y) override;
	void setX(float x) override;
	void setY(float y) override;
	void resize(float width, float height) override;
	void setWidth(float width) override;
	void setHeight(float height) override;
	void draw() override;
};