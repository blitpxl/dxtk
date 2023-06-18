#include "window.h"
#include "shitlog.h"

Window::Window()
: factory(NULL), renderTarget(NULL)
{
	is_window = true; 
	backgroundColor = D2D1::ColorF(D2D1::ColorF::Black);
}

HRESULT Window::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (renderTarget == NULL)
	{
		RECT rc;
		GetClientRect(GetHandle(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(GetHandle(), size),
			&renderTarget
		);

		hr = dwriteFactory->CreateTextFormat(
            L"Consolas",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            14.0f,
            L"", //locale
            &textFormat
        );

		textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

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
		BeginPaint(GetHandle(), &ps);

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
		EndPaint(GetHandle(), &ps);
	}
}

/*
bool intersecting(int x, int y, AABB aabb)
{
	return (x > aabb.left && x < aabb.right) && (y > aabb.top && y < aabb.bottom);
}
*/

void Window::OnMouseMove(int x, int y)
{
	for (MouseArea* mouseArea : mouseAreas)
	{
		if (mouseArea->mouseTracking)
		{
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
	
}

void Window::OnPrimaryMouseButtonUp(int x, int y)
{

}

void Window::OnSecondaryMouseButtonDown(int x, int y) { }  // overridable
void Window::OnSecondaryMouseButtonUp(int x, int y) { }  // overridable

void Window::OnResize()
{
	if (renderTarget != NULL)
	{
		RECT rc;
		GetClientRect(GetHandle(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		renderTarget->Resize(size);

		CalculateLayout((float)size.width, (float)size.height);
		InvalidateRect(GetHandle(), NULL, FALSE);
	}
}

void Window::push(Control* control)
{
	scene.push_back(control);
}

void Window::push(MouseArea* mouseArea)
{
	mouseAreas.push_back(mouseArea);
	scene.push_back(mouseArea);
}

void Window::redraw()
{
	InvalidateRect(GetHandle(), NULL, FALSE);
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
		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory)))
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
		return DefWindowProc(GetHandle(), msg, wparam, lparam);
	}
}