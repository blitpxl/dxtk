#pragma once
#include "rectangle.h"
#include "mousearea.h"

class Slider : public Rect
{
	float handlePos;
	float minValue;
	float maxValue;
public:
	float value;

	Rect* handle;
	Rect* valueBar;
	MouseArea* mouseArea;

	Slider(Control* parent, float x, float y, float width, float height);
	void setRange(float minValue, float maxValue);
	void setValue(float value);
};