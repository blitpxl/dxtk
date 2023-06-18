#pragma once
#include "abstractwindow.h"
#include "control.h"
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
protected:
	ID2D1Factory* factory;
	IDWriteFactory* dwriteFactory;
	D2D1_COLOR_F backgroundColor;

	std::vector<Control*> scene;

	virtual void Init() = 0;
	virtual void CalculateLayout(float width, float height) {};
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	virtual void OnPaint();
	virtual void OnMouseMove(int x, int y);
	virtual void OnPrimaryMouseButtonDown(int x, int y);
	virtual void OnPrimaryMouseButtonUp(int x, int y);
	virtual void OnSecondaryMouseButtonDown(int x, int y);
	virtual void OnSecondaryMouseButtonUp(int x, int y);
	void OnResize();
public:
	IDWriteTextFormat* textFormat;
	ID2D1HwndRenderTarget* renderTarget;

	Window();

	PCWSTR WindowClassName() const { return L"Window"; }
	LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};