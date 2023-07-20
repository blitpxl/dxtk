#pragma once
#include "abstractwindow.h"
#include "control.h"
#include "mousearea.h"
#include <windowsx.h>
#include <d2d1.h>
#include <dwrite.h>
#include <vector>
#include <set>

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

struct SetComparator {
   	bool operator() (Control* a, Control* b) const {
   	    return a->id < b->id;
   	}
};

class Window : public BaseWindow<Window>, public Control
{
	friend Control;
	friend MouseArea;

	bool customTitleBar;
	HCURSOR cursorShape = LoadCursor(NULL, IDC_ARROW);
	MouseArea* hoveredMouseArea;
protected:
	ID2D1Factory* factory;
	IDWriteFactory* dwriteFactory;

	std::set<Control*, SetComparator> scene;
	std::set<MouseArea*, SetComparator> mouseAreas;
	std::unordered_map<std::string_view, Control*> nameLookup;

	virtual void Init() = 0;
	void CalculateLayout(float width, float height);
	void OnPaint();
	void OnMouseMove(int x, int y);
	void OnPrimaryMouseButtonDown(int x, int y);
	void OnPrimaryMouseButtonUp(int x, int y);
	void OnSecondaryMouseButtonDown(int x, int y);
	void OnSecondaryMouseButtonUp(int x, int y);
	void OnKeyPress(WPARAM key);
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnResize();
public:
	ID2D1HwndRenderTarget* renderTarget;
	D2D1_COLOR_F backgroundColor;
	bool is_resizing;

	Window();

	void redraw();
	void setCursor(LPWSTR cursorName);
	void push(Control* control);
	void push(MouseArea* mouseArea);
	void windowStartMove();
	void setCustomTitleBar(bool enabled);
	PCWSTR WindowClassName() const { return L"Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};