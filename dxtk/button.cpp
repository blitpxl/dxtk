#include "button.h"
#include "window.h"

Button::Button(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height)
{
	stateColor.normal = Color(1.0f, 0.5f, 0.2f);
	stateColor.hover = Color(1.0f, 0.6f, 0.2f);
	stateColor.pressed = Color(0.8f, 0.3f, 0.0f);

	brush->SetColor(stateColor.normal);

	mouseArea = new MouseArea(this, 0, 0, 0, 0);
	mouseArea->setAnchor(AnchorType::fill);
	mouseArea->registerSignal("mouse_enter", [this](){ setColor(stateColor.hover); });
	mouseArea->registerSignal("mouse_leave", [this](){ setColor(stateColor.normal); });
	mouseArea->registerSignal("primary_button_down", [this](){ setColor(stateColor.pressed); invokeSignal("pressed"); });
	mouseArea->registerSignal("primary_button_up", [this](){ setColor(stateColor.hover); invokeSignal("clicked"); invokeSignal("released"); });

	label = new Label(this, 0, 0, 0, 0);
	label->setAnchor(AnchorType::fill);
	label->setText("Button");
}

void Button::setText(std::string const& text)
{
	label->setText(text);
}

void Button::setTextColor(D2D1_COLOR_F color)
{
	label->setColor(color);
}