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
	anchorPadding = 0;

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

	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;
	anchorPadding = 0;

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
}

void Control::requestRedraw()
{
	resource.window->redraw();
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
	case AnchorType::center:
		targetAnchors.horizontalCenter = AnchorType::horizontalCenter;
		targetAnchors.verticalCenter = AnchorType::verticalCenter;
		return;
	}
}

void Control::setAnchorPadding(float padding)
{
	anchorPadding = padding;
}

void Control::update()
{
	if (targetAnchors.fill != AnchorType::fill)
	{
		if (targetAnchors.left != AnchorType::none)
			x = parent->anchors[targetAnchors.left] + anchorPadding;
		else
			this->x = parent->x + localX;
	
		if (targetAnchors.top != AnchorType::none)
			y = parent->anchors[targetAnchors.top] + anchorPadding;
		else
			this->y = parent->y + localY;
	
		if (targetAnchors.right != AnchorType::none)
		{
			if (targetAnchors.left != AnchorType::none) // if the opposite anchor is set, then stretch the control rather than moving them
			{
				width = parent->width - (anchorPadding * 2);
			}
			else
			{
				x = (parent->anchors[targetAnchors.right] - width) - anchorPadding;
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
				y = (parent->anchors[targetAnchors.bottom] - height) - anchorPadding;
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
		this->x = (parent->x + localX) + anchorPadding;
		this->y = (parent->y + localY) + anchorPadding;
		width = parent->width - (anchorPadding * 2);
		height = parent->height - (anchorPadding * 2);
	}
	resize(width, height);

	anchors[AnchorType::left] = x;
	anchors[AnchorType::top] = y;
	anchors[AnchorType::right] = x + width;
	anchors[AnchorType::bottom] = y + height;
	anchors[AnchorType::horizontalCenter] = (anchors[AnchorType::right] - (width / 2));
	anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] - (height / 2);
}