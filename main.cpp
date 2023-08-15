#include <window.h>
#include <inputarea.h>
#include <line.h>

class MyWindow : public Window
{
public:
	void Init()
	{
		Line* line = new Line(this, Point(50, 50), Point(100, 100));
		line->setThickness(15.0f);
		line->setColor(Color(0xFF8484));
		registerSignal(this, "mouse_move", [this, line](){
			line->setPoints(Point(100, 100), this->mousePosition);
		});
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