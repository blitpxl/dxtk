#include "inputarea.h"
#include "window.h"

InputArea::InputArea(Control* parent, float x, float y, float width, float height)
: Control(parent), cursorName(IDC_ARROW), hasFocus(false), keyboardTracking(false), mouseTracking(true), 
  mouseEntered(false), dragging(false), mouseX(0), mouseY(0),
  draggable(false), dragPoint(0.0f, 0.0f), minDragX(-FLT_MAX), minDragY(-FLT_MAX),
  maxDragX(FLT_MAX), maxDragY(FLT_MAX)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	move(x, y);
	resource.window->push(this);

	resource.window->registerSignal(this, "primary_button_up", [this](){ dragging = false; });
	registerSignal(this, "mouse_enter", [this](){ resource.window->setCursor(cursorName); });
	registerSignal(this, "mouse_leave", [this](){ resource.window->setCursor(IDC_ARROW); });
	registerSignal(this, "mouse_drag", [this](){
		if (draggable)
		{
			this->parent->setX(std::clamp(mouseX - dragPoint.x, minDragX, maxDragX));
			this->parent->setY(std::clamp(mouseY - dragPoint.y, minDragY, maxDragY));
		}
	});
	z = zCounter;
	zCounter++;
}

InputArea::~InputArea()
{
	resource.window->unregisterSignal(this, "primary_button_up");
	resource.window->inputAreas.erase(this);
}

void InputArea::sendMouseDrag(int x, int y)
{
	if (dragging)
	{
		mouseX = x;
		mouseY = y;
		invokeSignal("mouse_drag");
	}
}

void InputArea::sendFocusGained()
{
	hasFocus = true;
	invokeSignal("focus_changed");
}

void InputArea::sendFocusLost()
{
	hasFocus = false;
	invokeSignal("focus_changed");
}

void InputArea::sendMouseEnter()
{
	mouseEntered = true;
	invokeSignal("mouse_enter");
}

void InputArea::sendMouseLeave()
{
	mouseEntered = false;
	invokeSignal("mouse_leave");
}

void InputArea::sendPrimaryMouseButtonDown(int x, int y)
{
	mouseX = x;
	mouseY = y;
	invokeSignal("primary_button_down");
	dragging = true;
	if (draggable)
		dragPoint = mapToLocal((float)x, (float)y);
}

void InputArea::sendPrimaryMouseButtonUp(int x, int y) { invokeSignal("primary_button_up"); dragging = false; }
void InputArea::sendSecondayMouseButtonDown(int x, int y) { invokeSignal("secondary_button_down"); }
void InputArea::sendSecondaryMouseButtonUp(int x, int y) { invokeSignal("secondary_button_up"); }

void InputArea::sendCharPress(wchar_t character)
{
	pressedChar = character;
	invokeSignal("char_pressed");
}

void InputArea::sendKeyPress(WPARAM key)
{
	pressedKey = key;
	invokeSignal("key_pressed");
}

bool InputArea::intersect(int x, int y)
{
	return (x > this->x && x < (this->x + width)) && (y > this->y && y < (this->y + height));
}

void InputArea::setCursor(LPWSTR cursorName)
{
	this->cursorName = cursorName;
}

void InputArea::setDraggable(bool draggable)
{
	this->draggable = draggable;
}

void InputArea::setDragLimitX(float minValue, float maxValue)
{
	minDragX = minValue;
	maxDragX = maxValue;
}

void InputArea::setDragLimitY(float minValue, float maxValue)
{
	minDragY = minValue;
	maxDragY = maxValue;
}