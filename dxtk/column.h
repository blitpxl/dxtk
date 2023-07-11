#pragma once
#include "control.h"

class Column : public Control
{
	std::vector<Control*> controls;
	float yPos;
public:
	float spacing;
	
	Column(Control* parent, float spacing = 8.0f);
	void add(Control* control);
	void setSpacing(float spacing);
};