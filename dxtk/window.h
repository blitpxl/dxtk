#pragma once
#include "abstractwindow.h"
#include "renderer.h"
#include "control.h"
#include "inputarea.h"

#include <windowsx.h>
#include <vector>
#include <set>
#include <thread>

struct OrderById {
   	bool operator() (Control* a, Control* b) const {
   	    return std::less<unsigned int>{}(a->id, b->id);
   	}
};

class Window : public BaseWindow<Window>, public Control
{
	friend Control;
	friend InputArea;

	bool customTitleBar;
	HCURSOR cursorShape = LoadCursor(NULL, IDC_ARROW);
	InputArea* hoveredInputArea;
	InputArea* focusedInputArea;
protected:
	std::set<Control*, OrderById> scene;
	std::set<InputArea*, OrderById> inputAreas;
	std::unordered_map<std::string_view, Control*> nameLookup;

	virtual void Init() = 0;
	void CalculateLayout(float width, float height);
	void OnPaint();
	void OnMouseMove(int x, int y);
	void OnPrimaryMouseButtonDown(int x, int y);
	void OnPrimaryMouseButtonUp(int x, int y);
	void OnSecondaryMouseButtonDown(int x, int y);
	void OnSecondaryMouseButtonUp(int x, int y);
	void OnCharPress(WPARAM character);
	void OnKeyPress(WPARAM key);
	void OnResize();
public:
	Renderer renderer;
	D2D1_COLOR_F backgroundColor;
	bool is_resizing;

	Window();

	void redraw();
	void setCursor(LPWSTR cursorName);
	void push(Control* control);
	void push(InputArea* inputArea);
	void windowStartMove();
	void setCustomTitleBar(bool enabled);
	void saveRenderState();
	void restoreRenderState();
	PCWSTR WindowClassName() const { return L"Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};