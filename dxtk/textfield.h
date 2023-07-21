#pragma once
#include "rectangle.h"
#include "label.h"
#include "inputarea.h"
#include "timer.h"

class TextField : public Rect
{
	bool is_caret_visible;
	unsigned int caret_position;

	Timer* caretTimer;
	std::string buffer;
public:
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