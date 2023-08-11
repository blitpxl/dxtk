#include <window.h>
#include <button.h>

class MyWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2f, 0.25f, 0.3f);

		Label* label = new Label(this, 0, 0, 0, 0);
		label->setAnchor(AnchorType::fill);
		label->setText("IAN GUANTENG");
		label->setScale(5.0f);
		label->setTextFormat("DIN");
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MyWindow window;
	window.create(L"DXTK Demo");
	window.setDark();
	window.show();
	window.init();
}