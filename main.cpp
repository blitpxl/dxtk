#include <window.h>

class MainWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2, 0.25, 0.3);
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"My Application", -1, -1, 900, 600);
	window.setDark();
	window.show();
	window.init();

	return 0;
}