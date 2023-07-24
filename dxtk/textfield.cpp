#include "textfield.h"

TextField::TextField(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), caret_position(0)
{
	setColor(Color(0.0f, 0.0f, 0.0f, 0.2f));

	text = new Label(this, 0, 0, 0, 0);
	text->setAnchor(AnchorType::fill);
	text->setAnchorPadding(8);
	text->setTextAlignment(TextAlignLeading);
	text->registerSignal(this, "text_format_changed", std::bind(&TextField::_onTextFormatChanged, this));

	inputArea = new InputArea(this, 0, 0, 0, 0);
	inputArea->keyboardTracking = true;
	inputArea->setCursor(IDC_IBEAM);
	inputArea->setAnchor(AnchorType::fill);
	inputArea->registerSignal(this, "char_pressed", std::bind(&TextField::_onCharPressed, this));
	inputArea->registerSignal(this, "key_pressed", std::bind(&TextField::_onKeyPressed, this));
	inputArea->registerSignal(this, "focus_changed", std::bind(&TextField::_onFocusChanged, this));

	caret = new Rect(text, 0, 0, 2, text->textFormat->GetFontSize() + 2);
	caret->setColor(Color(1.0f, 0.5f, 0.2f));
	caret->setVisible(false);
	caret->setAnchor(AnchorType::verticalCenter, AnchorType::verticalCenter);

	caretTimer = new Timer(500);
	caretTimer->registerSignal(this, "timeout", std::bind(&TextField::_caretBlink, this));

	_updateCaretPosition();
}

TextField::~TextField()
{
	delete caretTimer;
	delete text;
	delete inputArea;
	delete caret;

	text->unregisterSignal(this);
	inputArea->unregisterSignal(this);
	caretTimer->unregisterSignal(this);
}

void TextField::_onCharPressed()
{
	caret->setAnchor(AnchorType::verticalCenter, AnchorType::none);

	caret->setVisible(true);
	caretTimer->reset();
	if (inputArea->pressedChar == VK_BACK)
	{
		if (value.length() > 0 && caret_position > 0)
		{
			value.erase(caret_position-1, 1);
			text->setText(value);
			caret_position--;
			_updateCaretPosition();
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
		_updateCaretPosition();
	}
	invokeSignal("text_changed");
}

void TextField::_updateCaretPosition()
{
	FontMetrics metrics = text->getFontMetrics(caret_position);
	caret->setX(metrics.left);
	caret->setY(metrics.top + metrics.height - text->textFormat->GetFontSize() - 2);
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
		}
		break;

	case VK_LEFT:
		if (caret_position > 0)
		{
			caret_position--;
		}
		break;

	case VK_HOME:
		caret_position = 0;
		break;

	case VK_END:
		if (value.length() > 0)
		{
			caret_position = (unsigned int)value.length();
		}
		break;
	}
	_updateCaretPosition();
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

void TextField::_onTextFormatChanged()
{
	caret->setHeight(text->textFormat->GetFontSize() + 2);
}