#include <window.h>
#include <label.h>
#include <timer.h>

class MyApp : public Window
{
public:
	void Init()
	{
		auto label = new Label(this, DefaultPos, DefaultSize);
		label->setText("Hello DXTK!");
		label->setAnchor(AnchorType::fill);
		label->setScale(5.0f);

		auto timer = new Timer(14.0f);
		timer->registerSignal(this, "timeout", [timer, label](){
			label->setRotation(label->rotation+1.0f);
		});
		timer->start();
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MyApp app;
	app.create(L"My App", -1, -1, 900, 600);
	app.setDark();
	app.show();
	app.init();
}