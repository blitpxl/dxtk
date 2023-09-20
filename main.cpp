#include <window.h>
#include <textfield.h>
#include <timer.h>
#include <mathutils.h>
#include <cmath>

class MainWindow : public Window
{
	void Init()
	{
		auto timer = new Timer(15);

		backgroundColor = Color(0.22f, 0.25f, 0.30f);
		auto text = new TextField(this, DefaultPos, 300, 30);
		text->setAnchor(AnchorType::center);
		text->setBorderColor(Color(0.35f, 0.5f, 1.0f));
		text->text->setTextAlignment(TextAlignCenter);
		text->setRadius(4);

		timer->registerSignal(this, "timeout", [text, timer](){
			if (text->inputArea->mouseEntered)
			{
				text->setBorderWidth(std::lerp(text->borderWidth, 2.0f, 0.08f));
				if (equalf(text->borderWidth, 2.0f))
				{
					timer->stop();
				}
			}
			else
			{
				text->setBorderWidth(std::lerp(text->borderWidth, 0.0f, 0.1f));
				if (equalf(text->borderWidth, 0.0f))
				{
					timer->stop();
				}
			}
		});

		text->inputArea->registerSignal(this, "mouse_enter", [timer, text](){ timer->start(); text->setBorderWidth(8); });
		text->inputArea->registerSignal(this, "mouse_leave", [timer](){ timer->start(); });
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"DXTK Demo", -1, -1, 900, 600);
	window.setDark();
	window.show();
	window.init();
	return 0;
}