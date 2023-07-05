#include "mousearea.h"
#include "window.h"

MouseArea::MouseArea(Control* parent, float x, float y, float width, float height)
: Control(parent), cursorName(IDC_ARROW), mouseTracking(true), 
  mouseEntered(false), dragging(false), mouseX(0.0), mouseY(0.0),
  draggable(false), dragPoint(0, 0)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	move(x, y);
	resource.window->push(this);

	resource.window->registerSignal("primary_button_up", [this](){ dragging = false; });
	registerSignal("mouse_enter", [this](){ resource.window->setCursor(cursorName); });
	registerSignal("mouse_leave", [this](){ resource.window->setCursor(IDC_ARROW); });
	registerSignal("mouse_drag", [this](){
		if (draggable)
		{
			this->parent->move(mouseX - dragPoint.x, mouseY - dragPoint.y);
		}
	});
}

void MouseArea::sendMouseDrag(int x, int y)
{
	if (dragging)
	{
		mouseX = x;
		mouseY = y;
		invokeSignal("mouse_drag");
	}
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

void MouseArea::sendPrimaryMouseButtonDown(int x, int y)
{
	mouseX = x;
	mouseY = y;
	invokeSignal("primary_button_down");
	dragging = true;
	if (draggable)
		dragPoint = mapToLocal(x, y);
}

void MouseArea::sendPrimaryMouseButtonUp(int x, int y) { invokeSignal("primary_button_up"); dragging = false; }
void MouseArea::sendSecondayMouseButtonDown(int x, int y) { invokeSignal("secondary_button_down"); }
void MouseArea::sendSecondaryMouseButtonUp(int x, int y) { invokeSignal("secondary_button_up"); }

bool MouseArea::intersect(int x, int y)
{
	return (x > this->x && x < (this->x + width)) && (y > this->y && y < (this->y + height));
}

void MouseArea::setCursor(LPWSTR cursorName)
{
	this->cursorName = cursorName;
}

void MouseArea::setDraggable(bool draggable)
{
	this->draggable = draggable;
}