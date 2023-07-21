#pragma once
#include "rectangle.h"
#include "inputarea.h"
#include "mathutils.h"

class Slider : public Rect
{
	float handlePos;
	float minValue;
	float maxValue;
public:
	float value;

	Rect* handle;
	Rect* valueBar;
	InputArea* inputArea;

	Slider(Control* parent, float x, float y, float width, float height);
	~Slider();

	void setRange(float minValue, float maxValue);
	void setValue(float value);
};