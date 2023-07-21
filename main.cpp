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

		InputArea* inputArea = new InputArea(this, 0, 0, 0, 30);
		inputArea->setAnchor(AnchorType::left, AnchorType::left);
		inputArea->setAnchor(AnchorType::right, AnchorType::right);
		inputArea->registerSignal("primary_button_down", [this](){
			windowStartMove();
		});

		TextField* textField = new TextField(this, 0, 0, 300, 30);
		textField->setAnchor(AnchorType::top, AnchorType::top);
		textField->setAnchor(AnchorType::horizontalCenter, AnchorType::horizontalCenter);
		textField->setAnchorPadding(4);
		textField->setRadius(4);
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MyWindow window;
	window.create(L"DXTK Demo", -1, -1, 900, 600);
	window.setCustomTitleBar(true);
	window.setDark();
	window.show();
	window.init();
}