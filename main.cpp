#include <window.h>
#include <rectangle.h>
#include <row.h>
#include <inputarea.h>
#include <timer.h>
#include <mathutils.h>
#include <slider.h>

class MainWindow : public Window
{
	void Init()
	{
		backgroundColor = Color(0.22f, 0.25f, 0.30f);

		auto slider = new Slider(this, DefaultPos, 300, 10);
		slider->setAnchor(AnchorType::center);
		slider->setRange(0.0f, 200.0f);

		auto row = new Row(this, -100);
		//row->setAnchor(AnchorType::center, AnchorType::center);  

		auto timero = new Timer(12);
		auto timeri = new Timer(12);

		timero->registerSignal(this, "timeout", [timero, row](){
			if (!equalf(row->spacing, 16.0f))
				row->setSpacing(std::lerp(row->spacing, 16.0f, 0.1f));
			else
				timero->stop();
		});

		timeri->registerSignal(this, "timeout", [timeri, row](){
			if (!equalf(row->spacing, -100.0f))
				row->setSpacing(std::lerp(row->spacing, -100.0f, 0.1f));
			else
				timeri->stop();
		});

		auto input = new InputArea(row, DefaultPos, DefaultSize);
		input->setAnchor(AnchorType::fill);
		input->registerSignal(this, "mouse_enter", [slider, timero, timeri]() mutable {
			timeri->stop();
			timero->start();
		});
		input->registerSignal(this, "mouse_leave", [timeri, timero](){
			timero->stop();
			timeri->start();
		});

		for (int i = 0; i < 8; i++)
		{
			auto rect = new Rect(row, DefaultPos, DefaultSize);
			rect->setRadius(50);
			rect->setColor(RandomColor);
			row->add(rect);
		}
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