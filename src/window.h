#pragma once
#include <abstractwindow.h>
#include <control.h>
#include <mousearea.h>
#include <windowsx.h>
#include <d2d1.h>
#include <dwrite.h>
#include <vector>

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class Window : public BaseWindow<Window>, public Control
{
	HCURSOR cursorShape = LoadCursor(NULL, IDC_ARROW);
protected:
	ID2D1Factory* factory;
	IDWriteFactory* dwriteFactory;
	D2D1_COLOR_F backgroundColor;

	std::vector<Control*> scene;
	std::vector<MouseArea*> mouseAreas;

	virtual void Init() = 0;
	virtual void CalculateLayout(float width, float height) {};
	virtual void OnPaint();
	virtual void OnMouseMove(int x, int y);
	virtual void OnPrimaryMouseButtonDown(int x, int y);
	virtual void OnPrimaryMouseButtonUp(int x, int y);
	virtual void OnSecondaryMouseButtonDown(int x, int y);
	virtual void OnSecondaryMouseButtonUp(int x, int y);
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnResize();

	void push(Control* control);
	void push(MouseArea* mouseArea);
	void redraw();
	void setCursor(LPWSTR cursorName);
public:
	IDWriteTextFormat* textFormat;
	ID2D1HwndRenderTarget* renderTarget;

	Window();

	PCWSTR WindowClassName() const { return L"Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};