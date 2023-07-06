#include <window.h>
#include <slider.h>

class MainWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(0.2, 0.25, 0.3);

		Slider* sld = new Slider(this, 100, 100, 100, 10);
		sld->setAnchor(AnchorType::left, AnchorType::left);
		sld->setAnchor(AnchorType::right, AnchorType::right);
		sld->setAnchorPadding(48);

		Slider* slider = new Slider(this, 0, 0, 100, 10);
		slider->setAnchor(AnchorType::verticalCenter, AnchorType::verticalCenter);
		slider->setAnchor(AnchorType::left, AnchorType::left);
		slider->setAnchor(AnchorType::right, AnchorType::right);
		slider->setAnchorPadding(48);
		slider->registerSignal("value_changed", [slider, sld](){ sld->setValue(slider->value); });

		print(scene.size());
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"My Application", -1, -1, 900, 600);
	window.setDark();
	window.show();
	window.init();
	return 0;
}