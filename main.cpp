#include <window.h>
#include <textfield.h>
#include <button.h>

class MyWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2f, 0.25f, 0.3f);

		TextField* textField = new TextField(this, 0, 0, 300, 50);
		textField->setAnchor(AnchorType::fill);
		textField->setAnchorPadding(2);
		textField->setRadius(6);
		textField->text->setParagraphAlignment(ParagraphAlignNear);
		textField->text->setTextFormat("Segoe UI");
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