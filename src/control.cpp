#include "control.h"
#include "shitlog.h"

// this constructor should only be used by the window
Control::Control()
{
	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;
}

Control::Control(Control* parent)
{
	if (!parent->is_window)
		is_child = true;
	resource.renderTarget = parent->resource.renderTarget;
	this->parent = parent;

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

void Control::move(float x, float y)
{
	if (is_child)
	{
		this->localX = x;
		this->localY = y;
	}
	else
	{
		this->x = x;
		this->y = y;
	}
}

void Control::resize(float width, float height)
{
	this->width = width;
	this->height = height;
}

void Control::setAnchor(AnchorType controlAnchor, AnchorType targetParentAnchor)
{
	switch (controlAnchor)
	{
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
	if (is_child)
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
			if (targetAnchors.left != AnchorType::none)
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
		resize(width, height);
	}

	anchors[AnchorType::left] = x;
	anchors[AnchorType::top] = y;
	anchors[AnchorType::right] = x + width;
	anchors[AnchorType::bottom] = y + height;
	anchors[AnchorType::horizontalCenter] = anchors[AnchorType::right] / 2;
	anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] / 2;
}