#pragma once
#include <rectangle.h>
#include <mousearea.h>

class Button : public Rect, public MouseArea
{
public:
	Button(Control* parent, float x, float y, float width, float height);
};