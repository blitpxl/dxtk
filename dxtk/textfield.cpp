#include "textfield.h"

TextField::TextField(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), caret_position(0), selectionStart(0), selectionEnd(0), 
	is_selecting(false)
{
	setColor(Color(0.0f, 0.0f, 0.0f, 0.2f));

	text = new Label(this, 0, 0, 0, 0);
	text->setWrapMode(NoWrap);
	text->setAnchor(AnchorType::fill);
	text->setAnchorMargin(8);
	text->setTextAlignment(TextAlignLeading);
	text->registerSignal(this, "text_format_changed", [this](){ _onTextFormatChanged(); });
	text->addClipRectSource(this);

	inputArea = new InputArea(this, 0, 0, 0, 0);
	inputArea->keyboardTracking = true;
	inputArea->setCursor(IDC_IBEAM);
	inputArea->setAnchor(AnchorType::fill);
	inputArea->registerSignal(this, "char_pressed", [this](){ _onCharPressed(); });
	inputArea->registerSignal(this, "key_pressed", [this](){ _onKeyPressed(); });
	inputArea->registerSignal(this, "focus_changed", [this](){ _onFocusChanged(); });
	inputArea->registerSignal(this, "primary_button_down", [this](){ _onPrimaryDown(); });
	inputArea->registerSignal(this, "mouse_drag", [this](){ _onMouseDrag(); });

	selectionRect = new Rect(text, 0, 0, 0, 0);
	selectionRect->setColor(Color(1.0f, 1.0f, 1.0f, 0.2f));
	selectionRect->addClipRectSource(this);

	caret = new Rect(text, 0, 0, 2, text->textFormat->GetFontSize() + 2);
	caret->setColor(Color(1.0f, 0.5f, 0.2f));
	caret->setVisible(false);
	caret->setAnchor(AnchorType::verticalCenter, AnchorType::verticalCenter);
	caret->addClipRectSource(this);

	caretTimer = new Timer(500);
	caretTimer->registerSignal(this, "timeout", [this](){ _caretBlink(); });

	_updateCaretPosition();
}

TextField::~TextField()
{
	text->unregisterSignal(this);
	inputArea->unregisterSignal(this);
	caretTimer->unregisterSignal(this);

	delete caretTimer;
	delete text;
	delete inputArea;
	delete caret;
}

void TextField::_onCharPressed()
{
	caret->setAnchor(AnchorType::verticalCenter, AnchorType::none);

	if (is_selecting)
	{
		is_selecting = false;
		// the user selects from right to left, flip the selection start and end
		if (selectionStart > selectionEnd)
		{
			UINT32 temp = selectionEnd;
			selectionEnd = selectionStart;
			selectionStart = temp;

			/*
				if the user selects from right to left, the selection start (which now has been flipped and become selection end)
				will hit the next character's bounding box (we don't want that), so we decrement selectionEnd by 1
				so it doesn't delete the next character
				
				- rtl selection <- <- <-
				01234
				Hello
				  ^^
				selectionStart is now 4, selectionEnd is now 1
			*/

			if (selectionStart > 0)
				selectionStart += 1;
			selectionEnd -= 1;
		}

		value.erase(selectionStart, (selectionEnd - selectionStart) + 1);
		selectionRect->setVisible(false);

		caret_position = selectionStart;
		text->setText(value);
		_updateCaretPosition();
		if (inputArea->pressedChar == VK_BACK)
		{
			goto ret;  // skip everything, just invoke change signal and return
		}
	}

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

	ret:
	invokeSignal("text_changed");
}

void TextField::_updateCaretPosition()
{
	HitTestMetrics metrics = text->getFontMetrics(caret_position);
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
	if (caret->visible)
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

void TextField::_onPrimaryDown()
{
	is_selecting = false;
	selectionRect->setVisible(false);
	caret->setVisible(true);

	Point pt = mapToLocal((float)inputArea->mouseX, (float)inputArea->mouseY);
	HitInfo hitInfo = text->getPointHit(pt.x - text->margins.left, pt.y - text->margins.top);
	if (hitInfo.isTrailingHit)
	{
		caret->setX(hitInfo.metrics.left + hitInfo.metrics.width);
		selectionRect->setX(hitInfo.metrics.left + hitInfo.metrics.width);
		caret_position = hitInfo.metrics.textPosition + 1;
		selectionStart = caret_position;
	}
	else
	{
		caret->setX(hitInfo.metrics.left);
		selectionRect->setX(hitInfo.metrics.left);
		caret_position = hitInfo.metrics.textPosition;
		selectionStart = caret_position;
	}
}

void TextField::_onMouseDrag()
{
	is_selecting = true;
	selectionRect->setVisible(true);
	caret->setVisible(true);
	caretTimer->reset();

	Point pt = mapToLocal((float)inputArea->mouseX, (float)inputArea->mouseY);
	HitInfo hitInfo = text->getPointHit(pt.x - text->margins.left, pt.y - text->margins.top);
	if (hitInfo.isTrailingHit)
	{
		caret->setX(hitInfo.metrics.left + hitInfo.metrics.width);
		selectionRect->setWidth((hitInfo.metrics.left + hitInfo.metrics.width) - selectionRect->localX);
		selectionRect->setHeight(hitInfo.metrics.height);
		caret_position = hitInfo.metrics.textPosition + 1;
		selectionEnd = caret_position;
	}
	else
	{
		caret->setX(hitInfo.metrics.left);
		selectionRect->setWidth(hitInfo.metrics.left - selectionRect->localX);
		selectionRect->setHeight(hitInfo.metrics.height);
		caret_position = hitInfo.metrics.textPosition;
		selectionEnd = caret_position;
	}
	selectionRect->setY(hitInfo.metrics.top);
	if (selectionEnd > 0)
		selectionEnd -= 1;
	if (selectionStart == selectionEnd)
		is_selecting = false;
}