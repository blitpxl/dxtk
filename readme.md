# DXTK
Retained mode, Lightweight, Hardware-Accelerated and Highly customizable GUI Framework. It is heavily inspired by the Qt Framework and QML.

Features:
- **Lightweight**
	Lean memory usage and binary size.
- **Simple**
	Uses easy to understand concepts such as object signaling and event handling.
- **Hardware Accelerated**
	Uses the GPU to draw controls with fallback to software rendering when no GPU is present.
- **Smooth Animations**
	Controls are animated using floating point coordinate to achieve smooth animations.
	Controls can be easily animated with Timers and `std::lerp`.
- **Complex Layout with Anchors**
	An intuitive positioning concept inspired from Qt QML.

# Examples
### 1. Show an Empty Window

![an empty window](https://github.com/blitpxl/dxtk/blob/master/img/window.png?raw=true)

```cpp
// include the window header
#include <window.h>

// create custom class that subclasses Window
class MainWindow : public Window
{
public:
	void Init()
	{
		backgroundColor = Color(Color::Black);	// set the window background color

		// this is where you put your controls
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow window;
	window.create(L"DXTK Demo");
	window.setDark();	// set the titlebar to dark
	window.show();
	window.init();
	return 0;
}
```

### 2. Displaying text with Label
![a window showing a label](https://github.com/blitpxl/dxtk/blob/master/img/label.png?raw=true)

```cpp
#include <window.h>
#include <label.h>	// include the label header

class MainWindow : public Window
{
public:
	void Init()
	{
		// set the window background color
		backgroundColor = Color(Color::Black);

		// create a label with *this* (which is the window) as the parent.
		// and the x, y, width and height follows. in this case it's all set to 0
		Label* label = new Label(this, 0, 0, 0, 0);

		// set the label's text
		label->setText("Hello, world!");

		// set the font family and the font size
		label->setTextFormat("JetBrains Mono", 28.0f);

		// set the anchor to fill, which will make the label fill the parent, in this case the window.
		label->setAnchor(AnchorType::fill);
	}
};

...
```

### 3. Saying hello with Button
![a window with a button in it](https://github.com/blitpxl/dxtk/blob/master/img/button.png?raw=true)

```cpp
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

...
}
```

# The State
It is very crude.
I built this library to better understand how GUI Frameworks such as Qt and GTK works.

The current state of the project is still in a phase that I call "Good enough for small hobby projects but not for production and public use." as there's still so many Controls that I haven't implemented yet:

- [x] Label
- [x] Button
- [x] Horizontal Slider
- [ ] Vertical Slider
- [ ] Text Input
- [ ] CheckBox
- [ ] ComboBox
- [ ] Radio Button
- [ ] 
- [x] Row (need improvements)
- [x] Column (need improvements)
- [x] Rectangle
- [x] Rounded Rectangle
- [ ] Ellipse
- [x] Timer (need improvements)
- [x] Mouse Area
- [ ] ScrollView/Bar
