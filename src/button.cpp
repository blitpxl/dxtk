#include <button.h>
#include <window.h>

Button::Button(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height)
{
	stateColor.normal = Color(1.0, 0.5, 0.2);
	stateColor.hover = Color(1.0, 0.6, 0.2);
	stateColor.pressed = Color(0.8, 0.3, 0.0);
	brush->SetColor(stateColor.normal);

	mouseArea = new MouseArea(this, 0, 0, 0, 0);
	mouseArea->setAnchor(AnchorType::fill);
	mouseArea->registerSignal("mouse_enter", [this](){ resource.window->setCursor(IDC_HAND); setColor(stateColor.hover); });
	mouseArea->registerSignal("mouse_leave", [this](){ resource.window->setCursor(IDC_ARROW); setColor(stateColor.normal); });
	mouseArea->registerSignal("primary_button_down", [this](){ setColor(stateColor.pressed); invokeSignal("pressed"); });
	mouseArea->registerSignal("primary_button_up", [this](){ setColor(stateColor.hover); invokeSignal("clicked"); invokeSignal("released"); });

	label = new Label(this, 0, 0, 0, 0);
	label->setAnchor(AnchorType::fill);
	label->setText(L"Button");
}

void Button::setText(LPCWSTR text)
{
	label->setText(text);
}

void Button::setTextColor(D2D1_COLOR_F color)
{
	label->setColor(color);
}