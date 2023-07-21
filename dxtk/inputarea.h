#pragma once
#include "control.h"
#include "logging.h"
#include <algorithm>

class InputArea : public Control
{
	static inline int zCounter;
	LPWSTR cursorName;
	bool dragging;
	bool draggable;
	Point dragPoint;
public:
	bool hasFocus;
	bool keyboardTracking;
	bool mouseTracking;
	bool mouseEntered;
	float minDragX;
	float minDragY;
	float maxDragX;
	float maxDragY;
	int z;

	int mouseX;
	int mouseY;
	wchar_t pressedChar;
	WPARAM pressedKey;

	InputArea(Control* parent, float x, float y, float width, float height);
	~InputArea();

	void sendFocusGained();
	void sendFocusLost();
	void sendMouseEnter();
	void sendMouseLeave();
	void sendMouseDrag(int x, int y);
	void sendPrimaryMouseButtonDown(int x, int y);
	void sendPrimaryMouseButtonUp(int x, int y);
	void sendSecondayMouseButtonDown(int x, int y);
	void sendSecondaryMouseButtonUp(int x, int y);
	void sendCharPress(wchar_t character);
	void sendKeyPress(WPARAM key);

	bool intersect(int x, int y);
	void setCursor(LPWSTR cursorName);
	void setDraggable(bool draggable);
	void setDragLimitX(float minValue, float maxValue);
	void setDragLimitY(float minValue, float maxValue);
};