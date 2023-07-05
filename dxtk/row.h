#pragma once
#include "control.h"

class Row : public Control
{
	std::vector<Control*> controls;
	float xPos;
public:
	float spacing;
	
	Row(Control* parent, float spacing = 8.0f);
	void add(Control* control);
	void setSpacing(float spacing);
};