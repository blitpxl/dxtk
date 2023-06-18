#pragma once
#include <control.h>

class MouseArea : public Control
{
public:
	bool mouseTracking = true;
	bool mouseEntered = false;

	MouseArea(Control* parent, float x, float y, float width, float height);

	void sendMouseEnter();
	void sendMouseLeave();
	void sendPrimaryMouseButtonDown();
	void sendPrimaryMouseButtonUp();
	void sendSecondayMouseButtonDown();
	void sendSecondaryMouseButtonUp();
	bool intersect(int x, int y);
};