#include <window.h>
#include <button.h>

class MyWindow : public Window
{
public:
	std::vector<Control*> vectorControl;
	void Init()
	{
		backgroundColor = Color(0.2f, 0.25f, 0.3f);
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MyWindow window;
	window.create(L"My Window", -1, -1, 900, 600);
	window.setDark();
	window.show();
	window.init();
}