#pragma once
#include "rectangle.h"
#include "label.h"
#include "inputarea.h"

struct StateColor
{
	D2D1_COLOR_F normal;
	D2D1_COLOR_F hover;
	D2D1_COLOR_F pressed;
};

class Button : public Rect
{
public:
	InputArea* inputArea;
	Label* label;
	StateColor stateColor;

	Button(Control* parent, float x, float y, float width, float height);
	~Button();
	void destroy();

	void setText(std::string const& text);
	void setTextColor(ColorType color);

private:
	bool isPressed;
};