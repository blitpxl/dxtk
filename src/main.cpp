#include <window.h>
#include <shitlog.h>
#include <rectangle.h>
#include <mousearea.h>

class MainWindow : public Window
{
	Rect* rect;

	void Init()
	{
		backgroundColor = D2D1::ColorF(0.2, 0.25, 0.3);

		rect = new Rect(this, 100, 100, 100, 30);
		rect->setColor(D2D1::ColorF(1.0, 0.5, 0.0));
		push(rect);

		MouseArea* mouseArea = new MouseArea(rect, 0, 0, 100, 30);
		mouseArea->registerSignal("mouse_enter", [this](){ setCursor(IDC_HAND); });
		mouseArea->registerSignal("mouse_leave", [this](){ setCursor(IDC_ARROW); });
		push(mouseArea);
	}

	void CalculateLayout(float width, float height)
	{
		rect->move((width / 2) - rect->width / 2, (height / 2) - rect->height / 2);
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.Create(L"Direct2D ToolKit Demo", -1, -1, 900, 600);
	window.SetDark();
	ShowWindow(window.GetHandle(), nCmdShow);
	
	window.InitMessageLoop();

	return 0;
}