#include "renderer.h"

Renderer::Renderer()
: renderTarget(NULL) {}

void Renderer::initRenderer()
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory)))
	{
		print("Failed to create Direct2D Factory");
		return;
	}

	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(dwriteFactory),
		reinterpret_cast<IUnknown**>(&dwriteFactory)
	)))
	{
		print("Failed to create DirectWrite Factory");
		return;
	}
}

HRESULT Renderer::obtainGraphicsResources()
{
	HRESULT result = S_OK;
	if (renderTarget == NULL)
	{
		RECT clientRect;
		GetClientRect(windowHandle, &clientRect);
		D2D1_SIZE_U windowSize = D2D1::SizeU(clientRect.right, clientRect.bottom);

		result = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(), // add renderer option here (aka software render mode)
			D2D1::HwndRenderTargetProperties(windowHandle, windowSize),
			&renderTarget
		);
		if (FAILED(result))
			print("Failed to create Render Target");
		else
			result = factory->CreateDrawingStateBlock(&renderState);

		if (SUCCEEDED(result))
		{
			renderTargetSize = renderTarget->GetSize();
			invokeSignal("on_graphics_resources_created");
			invokeSignal("on_size_update");
		}
		else
		{
			print("Graphics resources creation failed");
			return E_FAIL;
		}
	}

	return result;
}

void Renderer::destroyGraphicsResources()
{
	SafeRelease(&renderTarget);
	SafeRelease(&renderState);
}

void Renderer::begin()
{
	BeginPaint(windowHandle, &paintStruct);
	renderTarget->BeginDraw();
}

void Renderer::end()
{
	HRESULT result = renderTarget->EndDraw();
	if (FAILED(result) || result == D2DERR_RECREATE_TARGET)
	{
		destroyGraphicsResources();
		invokeSignal("on_render_failed");
	}
	EndPaint(windowHandle, &paintStruct);
}

void Renderer::resize()
{
	if (renderTarget != NULL)
	{
		RECT clientRect;
		GetClientRect(windowHandle, &clientRect);
		D2D1_SIZE_U windowSize = D2D1::SizeU(clientRect.right, clientRect.bottom);
		renderTarget->Resize(windowSize);
		renderTargetSize = renderTarget->GetSize();
		invokeSignal("on_size_update");
	}
}