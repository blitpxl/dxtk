#include "window.h"

Window::Window()
: Control(), factory(NULL), renderTarget(NULL), renderer(DefaultRenderer),
is_resizing(false), hoveredInputArea(nullptr), focusedInputArea(nullptr),customTitleBar(false)
{
	is_window = true;
	backgroundColor = D2D1::ColorF(D2D1::ColorF::Black);
	resource.window = this;
	resource.renderTarget = renderTarget;
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
			D2D1::RenderTargetProperties(renderer),
			D2D1::HwndRenderTargetProperties(getHandle(), size),
			&renderTarget
		);

		hr = factory->CreateDrawingStateBlock(&renderState);

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
	// TODO: Fix auto fallback
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
			if (control->is_drawable && control->visible)
			{
				control->draw();
			}
		}

		hr = renderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(getHandle(), &ps);

		is_resizing = false; // if the window was resizing
	}
}

void Window::OnMouseMove(int x, int y)
{
	for (auto it = inputAreas.rbegin(); it != inputAreas.rend(); ++it)
	{
		InputArea* inputArea = *it;
		if (inputArea->mouseTracking)
		{
			inputArea->sendMouseDrag(x, y);

			if (inputArea->intersect(x, y))
			{
				if (hoveredInputArea && inputArea->z > hoveredInputArea->z)
				{
					hoveredInputArea->sendMouseLeave();
					hoveredInputArea = nullptr;
				}

				if (!inputArea->mouseEntered && !hoveredInputArea)
				{
					inputArea->sendMouseEnter();
					if (!inputArea->passThrough)
						hoveredInputArea = inputArea;
					break;
				}
			}
			else
			{
				if (inputArea->mouseEntered)
				{
					inputArea->sendMouseLeave();
					if (inputArea == hoveredInputArea)
						hoveredInputArea = nullptr;
					break;
				}
			}
		}
	}
}

void Window::OnPrimaryMouseButtonDown(int x, int y)
{
	for (auto it = inputAreas.rbegin(); it != inputAreas.rend(); ++it)
	{
		InputArea* inputArea = *it;
		if (inputArea->mouseTracking)
		{
			if (inputArea->intersect(x, y))
			{
				inputArea->sendPrimaryMouseButtonDown(x, y);
				
				if (focusedInputArea && focusedInputArea != inputArea)
				{
					focusedInputArea->sendFocusLost();
					focusedInputArea = nullptr;
				}
				if (!focusedInputArea)
				{
					inputArea->sendFocusGained();
					focusedInputArea = inputArea;
				}
				if (!inputArea->passThrough)
					break;
			}
			else
			{
				if (focusedInputArea)
				{
					focusedInputArea->sendFocusLost();
					focusedInputArea = nullptr;
				}
			}
		}
	}
	invokeSignal("primary_button_down");
}

void Window::OnPrimaryMouseButtonUp(int x, int y)
{
	for (auto it = inputAreas.rbegin(); it != inputAreas.rend(); ++it)
	{
		InputArea* inputArea = *it;
		if (inputArea->mouseTracking)
		{
			if (inputArea->intersect(x, y))
			{
				inputArea->sendPrimaryMouseButtonUp(x, y);
				if (!inputArea->passThrough)
					break;
			}
		}
	}
	invokeSignal("primary_button_up");
}

void Window::OnSecondaryMouseButtonDown(int x, int y)
{
	for (auto it = inputAreas.rbegin(); it != inputAreas.rend(); ++it)
	{
		InputArea* inputArea = *it;
		if (inputArea->mouseTracking)
		{
			if (inputArea->intersect(x, y))
			{
				inputArea->sendSecondaryMouseButtonDown(x, y);
				
				if (focusedInputArea && focusedInputArea != inputArea)
				{
					focusedInputArea->sendFocusLost();
					focusedInputArea = nullptr;
				}
				if (!focusedInputArea)
				{
					inputArea->sendFocusGained();
					focusedInputArea = inputArea;
				}
				if (!inputArea->passThrough)
					break;
			}
			else
			{
				if (focusedInputArea)
				{
					focusedInputArea->sendFocusLost();
					focusedInputArea = nullptr;
				}
			}
		}
	}
	invokeSignal("secondary_button_down");
}

void Window::OnSecondaryMouseButtonUp(int x, int y)
{
	for (auto it = inputAreas.rbegin(); it != inputAreas.rend(); ++it)
	{
		InputArea* inputArea = *it;
		if (inputArea->mouseTracking)
		{
			if (inputArea->intersect(x, y))
			{
				inputArea->sendSecondaryMouseButtonUp(x, y);
				if (!inputArea->passThrough)
					break;
			}
		}
	}
	invokeSignal("secondary_button_up");
}

void Window::OnCharPress(WPARAM character)
{
	for (InputArea* inputArea : inputAreas)
	{
		if (inputArea->keyboardTracking && inputArea->hasFocus)
		{
			inputArea->sendCharPress((wchar_t)character);
		}
	}
}

void Window::OnKeyPress(WPARAM key)
{
	for (InputArea* inputArea : inputAreas)
	{
		if (inputArea->keyboardTracking && inputArea->hasFocus)
		{
			inputArea->sendKeyPress(key);
		}
	}
}

void Window::OnResize()
{
	if (renderTarget != NULL)
	{
		RECT rc;
		GetClientRect(getHandle(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
		renderTarget->Resize(size);
		CalculateLayout((float)size.width, (float)size.height);

		is_resizing = true;
		redraw();
		invokeSignal("resize");
	}
}

void Window::push(Control* control)
{
	scene.insert(control);
}

void Window::push(InputArea* inputArea)
{
	inputAreas.insert(inputArea);
}

void Window::redraw()
{
	InvalidateRect(getHandle(), NULL, FALSE);
}

void Window::setCursor(LPWSTR cursorName)
{
	cursorShape = LoadCursor(NULL, cursorName);
}

void Window::windowStartMove()
{
	// using undocumented WM_SYSCOMMAND parameter from
	// https://stackoverflow.com/a/763273/21564282
	SendMessage(getHandle(), WM_SYSCOMMAND, SC_SIZE|0x9 , 0);
}

void Window::setCustomTitleBar(bool enabled)
{
	customTitleBar = enabled;
	SetWindowPos(getHandle(), nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}

void Window::saveRenderState()
{
	renderTarget->SaveDrawingState(renderState);
}

void Window::restoreRenderState()
{
	renderTarget->RestoreDrawingState(renderState);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		{
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
		resource.dwriteFactory = dwriteFactory;
		OnPaint();
		return 0;
	}

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

	case WM_NCCALCSIZE:
		{
			bool recalculate = static_cast<bool>(wparam);
            if (recalculate && customTitleBar)
            {
            	NCCALCSIZE_PARAMS* parameters = reinterpret_cast<NCCALCSIZE_PARAMS*>(lparam);
            	RECT& client_area = parameters->rgrc[0];
            	client_area.right -= GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
            	client_area.left += GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
            	client_area.bottom -= GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
            	return 0;
			}
			break;
		}

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
    	break;

    case WM_CHAR:
    	OnCharPress(wparam);
    	break;

    case WM_KEYDOWN:
    	OnKeyPress(wparam);
    	break;
	}

	return DefWindowProc(getHandle(), msg, wparam, lparam);
}