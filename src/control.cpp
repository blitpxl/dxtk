#include "control.h"
#include "shitlog.h"
#include <window.h>

// this constructor should only be used by the window
Control::Control()
{
	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;

	anchors[AnchorType::left] = x;
	anchors[AnchorType::top] = y;
	anchors[AnchorType::right] = x + width;
	anchors[AnchorType::bottom] = y + height;
	anchors[AnchorType::horizontalCenter] = anchors[AnchorType::right] / 2;
	anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] / 2;
}

Control::Control(Control* parent)
{
	resource.renderTarget = parent->resource.renderTarget;
	this->parent = parent;
	parent->registerSignal("on_update", [this](){ requestRedraw(); });

	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;

	anchors[AnchorType::left] = x;
	anchors[AnchorType::top] = y;
	anchors[AnchorType::right] = x + width;
	anchors[AnchorType::bottom] = y + height;
	anchors[AnchorType::horizontalCenter] = anchors[AnchorType::right] / 2;
	anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] / 2;
	resource.window->push(this);
}

void Control::move(float x, float y)
{
	this->localX = x;
	this->localY = y;
	redrawRequested = true;
}

void Control::requestRedraw()
{
	redrawRequested = true;
	resource.window->redraw();
}

void Control::resize(float width, float height)
{
	this->width = width;
	this->height = height;
	redrawRequested = true;
}

void Control::setAnchor(AnchorType controlAnchor, AnchorType targetParentAnchor)
{
	switch (controlAnchor)
	{
	case AnchorType::fill:
		targetAnchors.fill = AnchorType::fill;
		return;
	case AnchorType::left:
		targetAnchors.left = targetParentAnchor;
		return;
	case AnchorType::top:
		targetAnchors.top = targetParentAnchor;
		return;
	case AnchorType::right:
		targetAnchors.right = targetParentAnchor;
		return;
	case AnchorType::bottom:
		targetAnchors.bottom = targetParentAnchor;
		return;
	case AnchorType::horizontalCenter:
		targetAnchors.horizontalCenter = targetParentAnchor;
		return;
	case AnchorType::verticalCenter:
		targetAnchors.verticalCenter = targetParentAnchor;
		return;
	}
}

void Control::update()
{
	if (targetAnchors.fill != AnchorType::fill)
	{
		if (targetAnchors.left != AnchorType::none)
			x = parent->anchors[targetAnchors.left];
		else
			this->x = parent->x + localX;
	
		if (targetAnchors.top != AnchorType::none)
			y = parent->anchors[targetAnchors.top];
		else
			this->y = parent->y + localY;
	
		if (targetAnchors.right != AnchorType::none)
		{
			if (targetAnchors.left != AnchorType::none) // if the opposite anchor is set, then stretch the control rather than moving them
			{
				width = parent->width;
			}
			else
			{
				x = parent->anchors[targetAnchors.right] - width;
			}
		}

		if (targetAnchors.bottom != AnchorType::none)
		{
			if (targetAnchors.top != AnchorType::none)
			{
				height = parent->height;
			}
			else
			{
				y = parent->anchors[targetAnchors.bottom] - height;
			}
		}

		if (targetAnchors.horizontalCenter != AnchorType::none)
		{
			x = parent->anchors[targetAnchors.horizontalCenter] - (width / 2);
		}

		if (targetAnchors.verticalCenter != AnchorType::none)
		{
			y = parent->anchors[targetAnchors.verticalCenter] - (height / 2);
		}
	}
	else
	{
		this->x = parent->x + localX;
		this->y = parent->y + localY;
		width = parent->width;
		height = parent->height;
	}
	resize(width, height);

	anchors[AnchorType::left] = x;
	anchors[AnchorType::top] = y;
	anchors[AnchorType::right] = x + width;
	anchors[AnchorType::bottom] = y + height;
	anchors[AnchorType::horizontalCenter] = anchors[AnchorType::right] - (width / 2);
	anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] - (height / 2);
}