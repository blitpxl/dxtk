#pragma once
#include "control.h"

class MouseArea : public Control
{
	LPWSTR cursorName;
	bool dragging;
	bool draggable;
	Point dragPoint;
public:
	bool mouseTracking;
	bool mouseEntered;
	int mouseX;
	int mouseY;
	float minDragX;
	float minDragY;
	float maxDragX;
	float maxDragY;

	MouseArea(Control* parent, float x, float y, float width, float height);

	void sendMouseEnter();
	void sendMouseLeave();
	void sendMouseDrag(int x, int y);
	void sendPrimaryMouseButtonDown(int x, int y);
	void sendPrimaryMouseButtonUp(int x, int y);
	void sendSecondayMouseButtonDown(int x, int y);
	void sendSecondaryMouseButtonUp(int x, int y);
	bool intersect(int x, int y);
	void setCursor(LPWSTR cursorName);
	void setDraggable(bool draggable);
	void setDragLimitX(float minValue, float maxValue);
	void setDragLimitY(float minValue, float maxValue);
};