#include "window.h"

Window::Window()
: Control(), is_resizing(false), hoveredInputArea(nullptr),
  focusedInputArea(nullptr),customTitleBar(false)
{
	is_window = true;
	backgroundColor = D2D1::ColorF(D2D1::ColorF::Black);
	resource.window = this;

	renderer.registerSignal(this, "on_graphics_resources_created", [this](){
		resource.renderTarget = renderer.renderTarget;
	});

	renderer.registerSignal(this, "on_size_update", [this](){
		CalculateLayout(renderer.renderTargetSize.width, renderer.renderTargetSize.height);
		redraw();
	});
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

void Window::OnPaint()
{
	HRESULT hr = renderer.obtainGraphicsResources();
	if (SUCCEEDED(hr))
	{
		renderer.begin();
		renderer.renderTarget->Clear(backgroundColor);

		for (Control* control : scene)
		{
			control->update();
			if (control->is_drawable && control->visible)
			{
				control->draw();
			}
		}

		renderer.end();
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
	mousePosition.x = x;
	mousePosition.y = y;
	invokeSignal("mouse_move");
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
	is_resizing = true;
	renderer.resize();
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
	renderer.renderTarget->SaveDrawingState(renderer.renderState);
}

void Window::restoreRenderState()
{
	renderer.renderTarget->RestoreDrawingState(renderer.renderState);
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
		{
			renderer.windowHandle = getHandle();
			renderer.initRenderer();
			renderer.obtainGraphicsResources();
			resource.dwriteFactory = renderer.dwriteFactory;
			resource.imageFactory = renderer.imageFactory;
			Init();
			OnPaint();
			return 0;
		}

	case WM_DESTROY:
		renderer.destroyGraphicsResources();
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

    case WM_TIMER:
    	timers[wparam]->invokeSignal("timeout");
    	return 0;
	}

	return DefWindowProc(getHandle(), msg, wparam, lparam);
}