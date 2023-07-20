#include "control.h"
#include "window.h"

// this constructor should only be used by the window
Control::Control()
: name("unnamed")
{
	parent = NULL;
	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;
	anchorPadding = 0;
}

Control::Control(Control* parent)
: name("unnamed")
{
	x = 0;
	y = 0;
	localX = 0;
	localY = 0;
	width = 0;
	height = 0;
	anchorPadding = 0;
	this->parent = parent;
	resource.window->push(this);

	parent->registerSignal("dirty", [this](){ setDirty(); });
	instanceCounter++;
	id = instanceCounter;
}

Control::~Control()
{
	if (!is_window)
		resource.window->scene.erase(this);
	if (name != "unnamed")
		resource.window->nameLookup.erase(name);
	requestRedraw();
}

void Control::setName(std::string_view name)
{
	resource.window->nameLookup[name] = this;
	this->name = name;
}

void Control::requestRedraw()
{
	resource.window->redraw();
}

void Control::setDirty()
{
	is_dirty = true;
	invokeSignal("dirty");  // notify the children too
}

LPCWSTR Control::toWString(std::string const& string)
{
	int wchars_num = MultiByteToWideChar( CP_UTF8 , 0 , string.c_str() , -1, NULL , 0 );
	wchar_t* wstr = new wchar_t[wchars_num];
	MultiByteToWideChar( CP_UTF8 , 0 , string.c_str() , -1, wstr , wchars_num );
	return wstr;
}

Point Control::mapToLocal(float globalX, float globalY)
{
	Point result(0, 0);
	result.x = (globalX - x) + parent->parent->x;
	result.y = (globalY - y) + parent->parent->y;
	return result;
}

void Control::move(float x, float y)
{
	this->localX = x;
	this->localY = y;
	setDirty();
	invokeSignal("move");
}

void Control::setX(float x)
{
	this->localX = x;
	setDirty();
	invokeSignal("move");
}

void Control::setY(float y)
{
	this->localY = y;
	setDirty();
	invokeSignal("move");
}

void Control::resize(float width, float height)
{
	this->width = width;
	this->height = height;
	setDirty();
	invokeSignal("resize");
}

void Control::setWidth(float width)
{
	this->width = width;
	setDirty();
	invokeSignal("resize");
}

void Control::setHeight(float height)
{
	this->height = height;
	setDirty();
	invokeSignal("resize");
}

void Control::setAnchor(AnchorType controlAnchor, AnchorType targetParentAnchor)
{
	switch (controlAnchor)
	{
	case AnchorType::fill:
		targetAnchors.fill = AnchorType::fill;
		break;
	case AnchorType::left:
		targetAnchors.left = targetParentAnchor;
		break;
	case AnchorType::top:
		targetAnchors.top = targetParentAnchor;
		break;
	case AnchorType::right:
		targetAnchors.right = targetParentAnchor;
		break;
	case AnchorType::bottom:
		targetAnchors.bottom = targetParentAnchor;
		break;
	case AnchorType::horizontalCenter:
		targetAnchors.horizontalCenter = targetParentAnchor;
		break;
	case AnchorType::verticalCenter:
		targetAnchors.verticalCenter = targetParentAnchor;
		break;
	case AnchorType::center:
		targetAnchors.horizontalCenter = AnchorType::horizontalCenter;
		targetAnchors.verticalCenter = AnchorType::verticalCenter;
		break;
	}
	setDirty();
}

void Control::setAnchorPadding(float padding)
{
	anchorPadding = padding;
	setDirty();
}

void Control::update()
{
	if (is_dirty || resource.window->is_resizing)
	{
		if (targetAnchors.fill != AnchorType::fill)
		{
			if (targetAnchors.left != AnchorType::none)
				x = parent->anchors[targetAnchors.left] + anchorPadding;
			else
				x = parent->x + localX;
		
			if (targetAnchors.top != AnchorType::none)
				y = parent->anchors[targetAnchors.top] + anchorPadding;
			else
				y = parent->y + localY;
		
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
	
			if (targetAnchors.bottom != AnchorType::none) // same here but for top and bottom
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

		is_dirty = false;
	}
}

void Control::draw()
{
	// empty function to be overriden
}