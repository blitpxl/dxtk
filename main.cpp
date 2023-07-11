#include <window.h>
#include <button.h>	// include the button header

class MainWindow : public Window
{
public:
	void Init()
	{
		// set the window background color
		backgroundColor = Color(0.2f, 0.25f, 0.3f);

		// create a button with the window as its parent and set the width and the height.
		// we'll set the x and y to 0 as we use the anchors to position the control.
		Button* button = new Button(this, 0, 0, 120, 32);

		// set the button's text
		button->setText("Click me!");

		// set the button anchor to center so it stays centered in the window
		button->setAnchor(AnchorType::center);

		// register a signal to the button so that when you click the button,
		// it will change the button text to "Hello, world!"
		button->registerSignal("clicked", [button](){
			button->setText("Hello, world!");
		});

		// NOTE: you can register object signals to a lambda or a function with void(void) signature.
		// Signal can also have multiple functions/lambdas registered to it.
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"DXTK Demo");
	window.setDark();
	window.show();
	window.init();
	return 0;
}