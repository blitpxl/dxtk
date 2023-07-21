#include <window.h>
#include <textfield.h>
#include <button.h>

class MyWindow : public Window
{
public:
	std::vector<Control*> vectorControl;
	void Init()
	{
		backgroundColor = Color(0.2f, 0.25f, 0.3f);

		TextField* textField = new TextField(this, 0, 100, 300, 30);
		textField->text->setTextFormat("Open Sans");
		textField->setAnchor(AnchorType::horizontalCenter, AnchorType::horizontalCenter);
		textField->setRadius(4);

		TextField* textField1 = new TextField(this, 0, 150, 300, 30);
		textField1->setAnchor(AnchorType::horizontalCenter, AnchorType::horizontalCenter);

		Button* button = new Button(this, 0, 0, 100, 30);
		Button* button1 = new Button(this, 15, 15, 100, 30);
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