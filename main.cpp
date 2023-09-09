#include <window.h>
#include <button.h>

class MainWindow : public Window
{
	void Init()
	{
		Button* button = new Button(this, DefaultPos, 120, 30);
		button->setText("Close Window");
		button->setAnchor(AnchorType::center);
		button->registerSignal(this, "clicked", [this](){ close(); });
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"DXTK Demo");
	window.setDark();
	window.show();
	window.init();
	return 0;
}