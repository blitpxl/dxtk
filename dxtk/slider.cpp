#include "slider.h"

Slider::Slider(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), handlePos(0.0), minValue(0.0), maxValue(100.0), value(0.0)
{
	setColor(Color(1.0f, 0.5f, 0.2f, 0.5f));

	valueBar = new Rect(this, 0, 0, 0, 0);
	valueBar->setColor(Color(1.0f, 0.5f, 0.2f, 0.8f));
	valueBar->setAnchor(AnchorType::top, AnchorType::top);
	valueBar->setAnchor(AnchorType::bottom, AnchorType::bottom);
	valueBar->setAnchor(AnchorType::left, AnchorType::left);

	handle = new Rect(this, 0, 0, height*2, height*2);
	handle->setColor(Color(1.0f, 0.5f, 0.2f));
	handle->setAnchor(AnchorType::verticalCenter, AnchorType::verticalCenter);

	mouseArea = new MouseArea(handle, 0, 0, 0, 0);
	mouseArea->setAnchor(AnchorType::fill);
	mouseArea->setDraggable(true);

	mouseArea->registerSignal("mouse_drag", [this](){
		this->handlePos = mapRange(this->handle->localX, 0, this->width - this->handle->width, 0.0, 1.0);
		this->value = mapRange(this->handlePos, 0.0, 1.0, minValue, maxValue);
		this->invokeSignal("value_changed");
	});
	registerSignal("resize", [this](){
		mouseArea->setDragLimitX(0, this->width - this->handle->width);
		this->handle->setX(mapRange(this->handlePos, 0.0, 1.0, 0.0, this->width - this->handle->width));
		this->valueBar->setWidth(this->handle->localX);
	});
	registerSignal("value_changed", [this](){
		this->valueBar->setWidth(this->handle->localX);
	});
}

Slider::~Slider()
{
	delete handle;
	delete valueBar;
	delete mouseArea;
}

void Slider::setRange(float minValue, float maxValue)
{
	this->minValue = minValue;
	this->maxValue = maxValue;
}

void Slider::setValue(float value)
{
	this->handle->setX(mapRange(value, minValue, maxValue, 0.0, this->width - this->handle->width));
	this->handlePos = mapRange(value, minValue, maxValue, 0.0, 1.0);
	this->value = value;
	invokeSignal("value_changed");
}