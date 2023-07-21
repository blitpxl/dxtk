#include "textfield.h"

TextField::TextField(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), is_caret_visible(false), caret_position(0)
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
	caret->setColor(Color(1.0f, 0.5f, 0.2f, 0.0f));

	caretTimer = new Timer(500);
	caretTimer->registerSignal("timeout", std::bind(&TextField::_caretBlink, this));
}

void TextField::_onCharPressed()
{
	if (inputArea->pressedChar == VK_BACK)
	{
		if (buffer.length() > 0 && caret_position > 0)
		{
			buffer.erase(caret_position-1, 1);
			text->setText(buffer);
			caret_position--;
			caret->setX(text->getFontMetrics(caret_position).left);
		}
	}
	else
	{
		std::wstring wstr = std::wstring(1, inputArea->pressedChar);

		#pragma warning(suppress : 4244) // suppress utf-16 to utf-8 warning
		std::string character = std::string(wstr.begin(), wstr.end());
	
		buffer.insert(caret_position, character);
		text->setText(buffer);
		caret_position++;
	
		caret->setX(text->getFontMetrics(caret_position).left);
	}
}

void TextField::_onKeyPressed()
{
	if (inputArea->pressedKey == VK_RIGHT)
	{
		if (caret_position < buffer.length())
		{
			caret_position++;
			caret->setX(text->getFontMetrics(caret_position).left);
		}
	}
	else if (inputArea->pressedKey == VK_LEFT)
	{
		if (caret_position > 0)
		{
			caret_position--;
			caret->setX(text->getFontMetrics(caret_position).left);
		}
	}
}

void TextField::_caretBlink()
{
	if (is_caret_visible)
	{
		caret->setColor(Color(caret->color.r, caret->color.g, caret->color.b, 0.0f));
		is_caret_visible = false;
	}
	else
	{
		caret->setColor(Color(caret->color.r, caret->color.g, caret->color.b, 0.8f));
		is_caret_visible = true;
	}
}

void TextField::_onFocusChanged()
{
	if (inputArea->hasFocus)
	{
		caretTimer->start();
		caret->setColor(Color(caret->color.r, caret->color.g, caret->color.b, 0.8f));
		is_caret_visible = true;
	}
	else
	{
		caretTimer->stop();
		caret->setColor(Color(caret->color.r, caret->color.g, caret->color.b, 0.0f));
		is_caret_visible = false;
	}
}