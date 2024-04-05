#include <window.h>
#include <rectangle.h>
#include <line.h>
#include <label.h>
#include <image.h>
#include <button.h>



class DXWindow : public Rect
{
	InputArea* inputArea;

public:
	DXWindow(Control* parent, float x, float y, float width, float height)
	: Rect(parent, x, y, width, height)
	{
		auto line = new Line(this, Point(0, 30), Point(width, 30));
		line->setThickness(2.0f);
		line->setColor(Color(Color::Black));
		registerSignal(line, "resize", [line, this](){
			line->setPoints(Point(0, 30), Point(this->width, 30));
		});

		auto title = new Label(this, 0, 0, 0, 30);
		title->setAnchor(AnchorType::left, AnchorType::left);
		title->setAnchor(AnchorType::right, AnchorType::right);
		title->setAnchor(AnchorType::top, AnchorType::top);
		title->setText("Window");
		title->addClipRectSource(this);

		setColor(Color(0.3f, 0.3f, 0.3f));
		setBorderColor(Color(Color::Black));
		setBorderWidth(2.0);
		inputArea = new InputArea(this, 0, 0, 0, 30);
		inputArea->setAnchor(AnchorType::left, AnchorType::left);
		inputArea->setAnchor(AnchorType::right, AnchorType::right);
		inputArea->setDraggable(true);

		auto rightHandle = new InputArea(this, 0, 0, 16, 0);
		rightHandle->setAnchor(AnchorType::left, AnchorType::right);
		rightHandle->setAnchor(AnchorType::top, AnchorType::top);
		rightHandle->setAnchor(AnchorType::bottom, AnchorType::bottom);
		rightHandle->setAnchorMargin(-8);
		rightHandle->setCursor(IDC_SIZEWE);
		rightHandle->registerSignal(this, "mouse_drag", [rightHandle, this](){
			this->setWidth(this->width + rightHandle->mouseDelta.x);
		});

		auto leftHandle = new InputArea(this, 0, 0, 16, 0);
		leftHandle->setAnchor(AnchorType::right, AnchorType::left);
		leftHandle->setAnchor(AnchorType::top, AnchorType::top);
		leftHandle->setAnchor(AnchorType::bottom, AnchorType::bottom);
		leftHandle->setAnchorMargin(-8);
		leftHandle->setCursor(IDC_SIZEWE);
		leftHandle->registerSignal(this, "mouse_drag", [leftHandle, this](){
			this->setWidth(this->width - leftHandle->mouseDelta.x);
			this->setX(this->localX + leftHandle->mouseDelta.x);
		});

		auto btn = new Button(this, 0, 0, 100, 30);
		btn->setText("Close");
		btn->setAnchor(AnchorType::center);
		btn->addClipRectSource(this);
		btn->label->addClipRectSource(this);

		btn->registerSignal(this, "clicked", [this, line](){
			((Window*)this->parent)->close();
		});
	}
};


class MainWindow : public Window
{
	void Init()
	{
		backgroundColor = Color(0.15f, 0.15f, 0.15f);
		auto win = new DXWindow(this, 200, 200, 300, 250);
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