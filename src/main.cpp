#include <window.h>
#include <rectangle.h>
#include <mousearea.h>
#include <label.h>
#include <timer.h>
#include <button.h>
#include <cmath>
#include "mathutils.h"

class AnimatedRect : public Rect
{
public:
	bool mouse_hover = false;

	AnimatedRect(Control* parent, float x, float y, float width, float height)
	: Rect(parent, x, y, width, height)
	{
		Timer* timer = new Timer(16);
		timer->registerSignal("timeout", [this, timer](){
			if (mouse_hover)
			{
				resize(std::lerp(this->width, 250, 0.2), this->height);
				if (equalf(this->width, 250))
					timer->stop();
			}
			else
			{
				resize(std::lerp(this->width, 50, 0.2), this->height);
				if (equalf(this->width, 50))
					timer->stop();
			}
		});
		timer->start();

		MouseArea* mouseArea = new MouseArea(this, 0, 0, 0, 0);
		mouseArea->setAnchor(AnchorType::fill);
		mouseArea->registerSignal("mouse_enter", [this, timer](){ mouse_hover = true; timer->start(); });
		mouseArea->registerSignal("mouse_leave", [this, timer](){ mouse_hover = false; timer->start(); });
	}
};

class MainWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2, 0.25, 0.3);

		AnimatedRect* rect = new AnimatedRect(this, 0, 0, 50, 0);
		rect->setColor(Color(0.3, 0.35, 0.4));
		rect->setAnchor(AnchorType::left, AnchorType::left);
		rect->setAnchor(AnchorType::top, AnchorType::top);
		rect->setAnchor(AnchorType::bottom, AnchorType::bottom);

		int yPos = 8;
		for (int i = 0; i < 15; i++)
		{
			Button* button = new Button(rect, 0, yPos, 0, 30);
			button->setText(L"Button");
			button->setAnchorPadding(8);
			button->setAnchor(AnchorType::left, AnchorType::left);
			button->setAnchor(AnchorType::right, AnchorType::right);
			yPos+=38;
		}
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"My Application", -1, -1, 900, 600);
	window.setDark();
	ShowWindow(window.getHandle(), nCmdShow);

	window.initMessageLoop();

	return 0;
}