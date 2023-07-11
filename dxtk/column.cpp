#include "column.h"

Column::Column(Control* parent, float spacing)
: Control(parent), spacing(spacing), yPos(0) {}

void Column::add(Control* control)
{
	controls.push_back(control);
	if (controls.size() == 0)
	{
		control->setY(0);
	}
	else
	{
		control->setY(yPos);
		yPos+=controls.back()->height+spacing;
	}
}

void Column::setSpacing(float spacing)
{
	this->spacing = spacing;
	yPos = 0;
	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->setY(yPos);
		if (i != controls.size() - 1)
			yPos+=controls[i+1]->height+spacing;
	}
}