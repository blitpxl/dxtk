#include <mousearea.h>

MouseArea::MouseArea(Control* parent, float x, float y, float width, float height)
: Control(parent)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	move(x, y);
}

void MouseArea::sendMouseEnter()
{
	mouseEntered = true;
	invokeSignal("mouse_enter");
}

void MouseArea::sendMouseLeave()
{
	mouseEntered = false;
	invokeSignal("mouse_leave");
}

void MouseArea::sendPrimaryMouseButtonDown() { invokeSignal("primary_button_down"); }
void MouseArea::sendPrimaryMouseButtonUp() { invokeSignal("primary_button_up"); }
void MouseArea::sendSecondayMouseButtonDown() { invokeSignal("secondary_button_down"); }
void MouseArea::sendSecondaryMouseButtonUp() { invokeSignal("secondary_button_up"); }

bool MouseArea::intersect(int x, int y)
{
	return (x > this->x && x < (this->x + width)) && (y > this->y && y < (this->y + height));
}