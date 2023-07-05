#include "row.h"

Row::Row(Control* parent, float spacing)
: Control(parent), spacing(spacing), xPos(0) {}

void Row::add(Control* control)
{
	controls.push_back(control);
	if (controls.size() == 0)
	{
		control->setX(0);
	}
	else
	{
		control->setX(xPos);
		xPos+=controls.back()->width+spacing;
	}
}

void Row::setSpacing(float spacing)
{
	this->spacing = spacing;
	xPos = 0;
	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->setX(xPos);
		if (i != controls.size() - 1)
			xPos+=controls[i+1]->width+spacing;
	}
}