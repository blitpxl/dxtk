#include <window.h>
#include <button.h>
#include <label.h>

class DXButton : public Button
{
public:
	DXButton(Control* parent, float x, float y, float width, float height)
	: Button(parent, x, y, width, height)
	{
		Rect* overlay = new Rect(this, 0, 0, 0, 2);
		overlay->setColor(Color(0.51, 0.255, 0));
		overlay->setAnchor(AnchorType::left, AnchorType::left);
		overlay->setAnchor(AnchorType::right, AnchorType::right);
		overlay->setAnchor(AnchorType::top, AnchorType::bottom);
	}
};

class MainWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2, 0.25, 0.3);

		Rect* bottomPanel = new Rect(this, 0, 0, 0, 80);
		bottomPanel->setColor(Color(0.098, 0.122, 0.145));
		bottomPanel->setAnchor(AnchorType::bottom, AnchorType::bottom);
		bottomPanel->setAnchor(AnchorType::left, AnchorType::left);
		bottomPanel->setAnchor(AnchorType::right, AnchorType::right);

		DXButton* button = new DXButton(bottomPanel, 0, 0, 100, 32);
		button->setAnchor(AnchorType::verticalCenter, AnchorType::verticalCenter);
		button->setAnchor(AnchorType::horizontalCenter, AnchorType::horizontalCenter);
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