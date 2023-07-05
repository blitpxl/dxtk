#include "window.h"
#include "shitlog.h"

Window::Window()
: factory(NULL), renderTarget(NULL), Control()
{
	is_window = true;
	backgroundColor = D2D1::ColorF(D2D1::ColorF::Black);
	resource.window = this;
}

void Window::CalculateLayout(float width, float height)
{
	this->width = width;
	this->height = height;
	anchors[AnchorType::left] = 0;
	anchors[AnchorType::top] = 0;
	anchors[AnchorType::right] = width;
	anchors[AnchorType::bottom] = height;
	anchors[AnchorType::horizontalCenter] = anchors[AnchorType::right] - (width / 2);
	anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] - (height / 2);
}

HRESULT Window::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (renderTarget == NULL)
	{
		RECT rc;
		GetClientRect(getHandle(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			D2D1::HwndRenderTargetProperties(getHandle(), size),
			&renderTarget
		);

		if (SUCCEEDED(hr))
		{
			resource.renderTarget = renderTarget;

			if (scene.size() == 0)
			{
				Init();
			}
			
			D2D1_SIZE_F size = renderTarget->GetSize();
			CalculateLayout(size.width, size.height);
		}
	}

	return hr;
}

void Window::DiscardGraphicsResources()
{
	SafeRelease(&renderTarget);
}

void Window::OnPaint()
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(getHandle(), &ps);

		renderTarget->BeginDraw();

		renderTarget->Clear(backgroundColor);

		for (Control* control : scene)
		{
			control->update();
		}

		hr = renderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(getHandle(), &ps);
	}
}

void Window::OnMouseMove(int x, int y)
{
	for (MouseArea* mouseArea : mouseAreas)
	{
		if (mouseArea->mouseTracking)
		{
			mouseArea->sendMouseDrag(x, y);

			if (mouseArea->intersect(x, y))
			{
				if (!mouseArea->mouseEntered)
				{
					mouseArea->sendMouseEnter();
				}
			}
			else
			{
				if (mouseArea->mouseEntered)
				{
					mouseArea->sendMouseLeave();
				}
			}
		}
	}
}

void Window::OnPrimaryMouseButtonDown(int x, int y)
{
	for (MouseArea* mouseArea : mouseAreas)
	{
		if (mouseArea->mouseTracking)
		{
			if (mouseArea->intersect(x, y))
			{
				mouseArea->sendPrimaryMouseButtonDown(x, y);
			}
		}
	}
	invokeSignal("primary_button_down");
}

void Window::OnPrimaryMouseButtonUp(int x, int y)
{
	for (MouseArea* mouseArea : mouseAreas)
	{
		if (mouseArea->mouseTracking)
		{
			if (mouseArea->intersect(x, y))
			{
				mouseArea->sendPrimaryMouseButtonUp(x, y);
			}
		}
	}
	invokeSignal("primary_button_up");
}

void Window::OnSecondaryMouseButtonDown(int x, int y) { }  // overridable
void Window::OnSecondaryMouseButtonUp(int x, int y) { }  // overridable

void Window::OnResize()
{
	if (renderTarget != NULL)
	{
		RECT rc;
		GetClientRect(getHandle(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		renderTarget->Resize(size);

		CalculateLayout((float)size.width, (float)size.height);

		redraw();
	}
}

void Window::push(Control* control)
{
	scene.push_back(control);
}

void Window::push(MouseArea* mouseArea)
{
	mouseAreas.push_back(mouseArea);
}

void Window::redraw()
{
	InvalidateRect(getHandle(), NULL, FALSE);
}

void Window::setCursor(LPWSTR cursorName)
{
	cursorShape = LoadCursor(NULL, cursorName);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory)))
		{
			return -1;
		}

		if (FAILED(DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(dwriteFactory),
			reinterpret_cast<IUnknown **>(&dwriteFactory)
		)))
		{
			return -1;
		}
		resource.dwriteFactory = dwriteFactory;
		OnPaint();
		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&factory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_SIZE:
		OnResize();
		return 0;

	case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;

	case WM_LBUTTONDOWN:
		OnPrimaryMouseButtonDown(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;

	case WM_LBUTTONUP:
		OnPrimaryMouseButtonUp(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;

	case WM_RBUTTONDOWN:
		OnSecondaryMouseButtonDown(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;

	case WM_RBUTTONUP:
		OnSecondaryMouseButtonUp(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;

	case WM_SETCURSOR:
    	if (LOWORD(lparam) == HTCLIENT)
    	{
    		SetCursor(cursorShape);
    		return TRUE;
    	}

	default:
		return DefWindowProc(getHandle(), msg, wparam, lparam);
	}
}