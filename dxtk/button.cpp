#include "button.h"
#include "window.h"

Button::Button(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), isPressed(false)
{
	stateColor.normal = Color(1.0f, 0.5f, 0.2f);
	stateColor.hover = Color(1.0f, 0.6f, 0.2f);
	stateColor.pressed = Color(0.8f, 0.3f, 0.0f);

	brush->SetColor(stateColor.normal);

	inputArea = new InputArea(this, 0, 0, 0, 0);
	inputArea->setAnchor(AnchorType::fill);
	inputArea->registerSignal(this, "mouse_enter", [this](){ setColor(stateColor.hover); });
	inputArea->registerSignal(this, "mouse_leave", [this](){ setColor(stateColor.normal); this->isPressed = false; });
	inputArea->registerSignal(this, "primary_button_down", [this](){ setColor(stateColor.pressed); invokeSignal("pressed"); });
	inputArea->registerSignal(this, "primary_button_up", [this](){ setColor(stateColor.hover); invokeSignal("released"); });

	registerSignal(this, "pressed", [this](){ isPressed = true; });
	registerSignal(this, "released", [this](){
		if (isPressed)
			invokeSignal("clicked");
	});

	label = new Label(this, 0, 0, 0, 0);
	label->setAnchor(AnchorType::fill);
	label->setText("Button");
}

Button::~Button()
{
	delete inputArea;
	delete label;
}

void Button::setText(std::string const& text)
{
	label->setText(text);
}

void Button::setTextColor(ColorType color)
{
	label->setColor(color);
}