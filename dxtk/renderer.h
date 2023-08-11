#pragma once
#include "dxobject.h"
#include "logging.h"

#include <d2d1.h>
#include <dwrite.h>

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class Renderer : public DxObject
{
public:
	HWND windowHandle;
	ID2D1Factory* factory;
	IDWriteFactory* dwriteFactory;
	ID2D1HwndRenderTarget* renderTarget;
	ID2D1DrawingStateBlock* renderState;
	D2D1_SIZE_F renderTargetSize;

public:
	Renderer();
	void initRenderer();
	HRESULT obtainGraphicsResources();
	void destroyGraphicsResources();
	void begin();
	void end();
	void resize();

private:
	PAINTSTRUCT paintStruct;
};