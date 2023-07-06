#pragma once
#include "rectangle.h"
#include "label.h"
#include "mousearea.h"

struct StateColor
{
	D2D1_COLOR_F normal;
	D2D1_COLOR_F hover;
	D2D1_COLOR_F pressed;
};

class Button : public Rect
{
public:
	MouseArea* mouseArea;
	Label* label;
	StateColor stateColor;

	Button(Control* parent, float x, float y, float width, float height);
	void setText(LPCWSTR text);
	void setTextColor(D2D1_COLOR_F color);
};