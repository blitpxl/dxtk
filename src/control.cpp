#include "control.h"
#include "shitlog.h"

// this constructor should only be used by the window
Control::Control()
{
	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;
}

Control::Control(Control* parent)
{
	if (!parent->is_window)
		is_child = true;
	resource.renderTarget = parent->resource.renderTarget;
	this->parent = parent;

	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;
}

void Control::move(float x, float y)
{
	if (is_child)
	{
		this->localX = x;
		this->localY = y;
	}
	else
	{
		this->x = x;
		this->y = y;
	}
}

void Control::resize(float width, float height)
{
	this->width = width;
	this->height = height;
}

void Control::update()
{
	if (is_child)
	{
		printd(localX);
		this->x = parent->x + localX;
		this->y = parent->y + localY;
	}
}