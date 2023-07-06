#include <window.h>
#include <button.h>

class MainWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2f, 0.25f, 0.3f);

		int w = 900;
		int h = 600;
		int size = 15;
		for (int i = 0; i < w; i+=size)
		{
			for (int j = 0; j < h; j+=size)
			{
				Button* button = new Button(this, i, j, size, size);
				button->setText(L"B");
				button->registerSignal("clicked", [this](){ this->setSize(150, 150); });
			}
		}
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