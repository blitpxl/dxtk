/*
	Drawing bunch of rectangles using Shapes class and ImmediateDraw function

	---------------------------
	Kevin Putra Satrianto, 2023
*/

#include <window.h>
#include <shapes.h>
#include <mathutils.h>
#include <timer.h>
#include <label.h>
#include <cmath>

class MainWindow : public Window
{
	// size of individual rectangles
	const float w = 4.0f;
	const float h = 4.0f;

	// spinning velocity and spacing of rectangles
	const float velocity = 0.087f;
	const float spacing_inc = 2.0f;

	Shapes* shapes;

	float radius = 0.0f;
	int n = 0;
	int frame_count = 0;

	void Init()
	{
		// class to draw shapes
		shapes = new Shapes(this);

		// FPS counter label
		Label* label = new Label(this, DefaultPos, 100, 30);
		label->setText("0 FPS");

		// create timer to update FPS counter
		Timer* timer = new Timer(1000);
		timer->registerSignal(this, "timeout", [this, label](){
			label->setText(std::to_string(frame_count) + " FPS");
			frame_count = 0;
		});
		timer->start();
	}

	void ImmediateDraw()
	{
		float spacing = 0.0f;
		for (int i = 0; i < 5000; i++)
		{
			ColorType color = Color(
				mapRange((float)i, 0, 5000, 1.0f, 1.0f),
				mapRange((float)i, 0, 5000, 0.0f, 1.0f),
				mapRange((float)i, 0, 5000, 1.0f, 0.0f)
			);
			float x = std::cos(mapRange((float)i, 0.0f, 1.0f, 0.01f, 0.1f) * velocity * 0.01f * n) * (radius + (spacing));
			float y = std::sin(mapRange((float)i, 0.0f, 1.0f, 0.01f, 0.1f) * velocity * 0.01f * n) * (radius + (spacing));
			shapes->DrawRect((width/2) + x - (w/2), (height/2) + y - (h/2), w, h, color);
			spacing += spacing_inc;
		}
		n++;
		frame_count++;
		redraw();
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"DXTK Demo: Spiral", -1, -1, 900, 600);
	window.setDark();
	window.show();
	window.init();
	return 0;
}