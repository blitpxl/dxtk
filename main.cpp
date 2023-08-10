#include <window.h>
#include <textfield.h>
#include <button.h>

class MyWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2f, 0.25f, 0.3f);

		TextField* textField = new TextField(this, 0, 0, 300, 32);
		textField->setAnchor(AnchorType::center);
		textField->setRadius(4);
		textField->selectionRect->setRadius(2);
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MyWindow window;
	window.create(L"DXTK Demo", -1, -1, 900, 600);
	window.setDark();
	window.show();
	window.init();
}