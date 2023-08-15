#pragma once
#include "dxobject.h"
#include "logging.h"

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include "d2dalias.h"

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
	IWICImagingFactory* imageFactory;

	ID2D1HwndRenderTarget* renderTarget;
	ID2D1DrawingStateBlock* renderState;
	D2D1_SIZE_F renderTargetSize;
	float scaling;

public:
	Renderer();
	void initRenderer();
	HRESULT obtainGraphicsResources();
	void destroyGraphicsResources();
	void setRenderScale(float scaling);
	void begin();
	void end();
	void resize();

private:
	PAINTSTRUCT paintStruct;
};