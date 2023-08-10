#pragma once
#include "rectangle.h"
#include "label.h"
#include "inputarea.h"
#include "timer.h"
#include "mathutils.h"

class TextField : public Rect
{
	unsigned int caret_position;
	Timer* caretTimer;
	bool caret_fading;
	bool is_selecting;
	UINT32 selectionStart;
	UINT32 selectionEnd;
public:
	std::string value;
	Label* text;
	InputArea* inputArea;
	Rect* caret;
	Rect* selectionRect;

	TextField(Control* parent, float x, float y, float width, float height);
	~TextField();

private:
	void _updateCaretPosition();
	void _onCharPressed();
	void _onKeyPressed();
	void _caretBlink();
	void _onFocusChanged();
	void _onTextFormatChanged();
	void _onPrimaryDown();
	void _onMouseDrag();
};