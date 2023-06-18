#include <window.h>
#include <shitlog.h>
#include <rectangle.h>

class MainWindow : public Window
{
	Rect* rect;

	void Init()
	{
		rect = new Rect(this, 100, 100, 350, 350);
		rect->setColor(D2D1::ColorF(1.0, 0.5, 0.0));
		scene.push_back(rect);

		Rect* rect1 = new Rect(rect, 10, 10, 100, 100);
		scene.push_back(rect1);
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