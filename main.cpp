#include <window.h>
#include <textfield.h>
#include <line.h>

class MyWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.15f, 0.15f, 0.15f);

		Line* line = new Line(this, Point(50, 50), Point(100, 100));
		line->setThickness(15.0f);
		line->setColor(Color(0xFF8484));
		line->setPoints(Point(50, 50), Point(100, 200));

		TextField* field = new TextField(this, 0, 0, 300, 40);
		field->setAnchor(AnchorType::center);
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