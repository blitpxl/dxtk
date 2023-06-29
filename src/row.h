#include "control.h"

class Row : public Control
{
	std::vector<Control*> controls;

public:
	Row(Control* parent);
}