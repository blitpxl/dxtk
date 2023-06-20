#pragma once
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN
#define NOCOMM

#include <windows.h>
#include <dwmapi.h>


template <class DERIVED>
class BaseWindow
{
protected:
	HWND handle;
	int x, y, w, h;
public:
	static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		DERIVED* pThis = NULL;
	
		if (msg == WM_CREATE)
		{
			CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lparam);
			pThis = reinterpret_cast<DERIVED*>(cs->lpCreateParams);
			SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->handle = handle;
		}
		else
		{
			pThis = (DERIVED*)GetWindowLongPtr(handle, GWLP_USERDATA);
		}
	
		if (pThis)
			return pThis->HandleMessage(msg, wparam, lparam);
		else
			return DefWindowProc(handle, msg, wparam, lparam);
	}

	BaseWindow() : handle(NULL) { }

	BOOL Create(LPCWSTR windowName, int x = -1, int y = -1, int w = -1, int h = -1)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;

		if (w < 0)
			this->w = 640;
		if (h < 0)
			this->h = 480;
		if (x < 0)
			this->x = (GetSystemMetrics(SM_CXFULLSCREEN) / 2) - (this->w / 2);
		if (y < 0)
			this->y = (GetSystemMetrics(SM_CYFULLSCREEN) / 2) - (this->h / 2);

		WNDCLASS wc = { };
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = WindowClassName();
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpfnWndProc = DERIVED::WndProc;
		RegisterClass(&wc);
	
		handle = CreateWindowEx(
			0,
			WindowClassName(),
			windowName,
			WS_OVERLAPPEDWINDOW,
			this->x, this->y, this->w, this->h,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			this
		);

		return (handle ? TRUE : FALSE);
	}

	void SetDark()
	{
		BOOL dark = TRUE;
		DwmSetWindowAttribute(GetHandle(), 20, &dark, sizeof(dark));
	}

	void Move(int x, int y)
	{
		MoveWindow(GetHandle(), x, y, w, h, FALSE);
	}

	void Resize(int w, int h)
	{
		MoveWindow(GetHandle(), x, y, w, h, FALSE);
	}

	HWND GetHandle() const { return handle; }

	void InitMessageLoop()
	{
		MSG msg = { };
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	virtual PCWSTR WindowClassName() const = 0;
	virtual LRESULT HandleMessage(UINT msg, WPARAM wparam, LPARAM lparam) = 0;
};