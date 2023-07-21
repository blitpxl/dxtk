#pragma once
#include "rectangle.h"
#include "label.h"
#include "inputarea.h"
#include "timer.h"

class TextField : public Rect
{
	unsigned int caret_position;
	Timer* caretTimer;
public:
	std::string value;
	Label* text;
	InputArea* inputArea;
	Rect* caret;

	TextField(Control* parent, float x, float y, float width, float height);

private:
	void _onCharPressed();
	void _onKeyPressed();
	void _caretBlink();
	void _onFocusChanged();
};