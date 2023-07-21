#include "textfield.h"

TextField::TextField(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), caret_position(0)
{
	setColor(Color(0.0f, 0.0f, 0.0f, 0.2f));

	text = new Label(this, 0, 0, 0, 0);
	text->setAnchor(AnchorType::fill);
	text->setAnchorPadding(8);
	text->setTextAlignment(TextAlignLeading);

	inputArea = new InputArea(this, 0, 0, 0, 0);
	inputArea->keyboardTracking = true;
	inputArea->setCursor(IDC_IBEAM);
	inputArea->setAnchor(AnchorType::fill);
	inputArea->registerSignal("char_pressed", std::bind(&TextField::_onCharPressed, this));
	inputArea->registerSignal("key_pressed", std::bind(&TextField::_onKeyPressed, this));
	inputArea->registerSignal("focus_changed", std::bind(&TextField::_onFocusChanged, this));

	caret = new Rect(text, 0, 0, 2, 0);
	caret->setAnchor(AnchorType::top, AnchorType::top);
	caret->setAnchor(AnchorType::bottom, AnchorType::bottom);
	caret->setColor(Color(1.0f, 0.5f, 0.2f));
	caret->is_drawable = false;

	caretTimer = new Timer(500);
	caretTimer->registerSignal("timeout", std::bind(&TextField::_caretBlink, this));
}

void TextField::_onCharPressed()
{
	caret->setVisible(true);
	caretTimer->reset();
	if (inputArea->pressedChar == VK_BACK)
	{
		if (value.length() > 0 && caret_position > 0)
		{
			value.erase(caret_position-1, 1);
			text->setText(value);
			caret_position--;
			caret->setX(text->getFontMetrics(caret_position).left);
		}
	}
	else
	{
		std::wstring wstr = std::wstring(1, inputArea->pressedChar);

		#pragma warning(suppress : 4244) // suppress utf-16 to utf-8 warning
		std::string character = std::string(wstr.begin(), wstr.end());
	
		value.insert(caret_position, character);
		text->setText(value);
		caret_position++;
	
		caret->setX(text->getFontMetrics(caret_position).left);
	}
	invokeSignal("text_changed");
}

void TextField::_onKeyPressed()
{
	caret->setVisible(true);
	caretTimer->reset();

	switch (inputArea->pressedKey)
	{
	case VK_RIGHT:
		if (caret_position < value.length())
		{
			caret_position++;
			caret->setX(text->getFontMetrics(caret_position).left);
		}
		break;

	case VK_LEFT:
		if (caret_position > 0)
		{
			caret_position--;
			caret->setX(text->getFontMetrics(caret_position).left);
		}
		break;

	case VK_HOME:
		caret_position = 0;
		caret->setX(text->getFontMetrics(caret_position).left);
		break;

	case VK_END:
		if (value.length() > 0)
		{
			caret_position = (unsigned int)value.length();
			caret->setX(text->getFontMetrics(caret_position).left);
		}
		break;
	}
}

void TextField::_caretBlink()
{
	if (caret->is_drawable)
	{
		caret->setVisible(false);
	}
	else
	{
		caret->setVisible(true);
	}
}

void TextField::_onFocusChanged()
{
	if (inputArea->hasFocus)
	{
		caretTimer->start();
		caret->setVisible(true);
	}
	else
	{
		caretTimer->stop();
		caret->setVisible(false);
	}
}