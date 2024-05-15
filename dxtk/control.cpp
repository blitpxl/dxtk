#include "control.h"
#include "window.h"

// this constructor should only be used by the window
Control::Control()
: x(0), y(0), localX(0), localY(0), width(0), height(0), visible(false)
{
	name = "unnamed";
	parent = NULL;
}

Control::Control(Control* parent)
: x(0), y(0), localX(0), localY(0), width(0), height(0), visible(false)
{
	name = "unnamed";

	instanceCounter++;
	id = instanceCounter;

	this->parent = parent;
	resource.window->push(this);

	// propagate dirty signal from parent to children
	parent->registerSignal(this, "dirty", [this](){ setDirty(); });
}

Control::~Control()
{
	if (!is_window)
		resource.window->scene.erase(this);
	if (name != "unnamed")
		resource.window->nameLookup.erase(name);
	requestRedraw();
}

void Control::setVisible(bool visible)
{
	if (this->visible != visible)
	{
		this->visible = visible;
		requestRedraw();
	}
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
	if (!parent->is_window)
	{
		result.x = (globalX - x) + parent->parent->x;
		result.y = (globalY - y) + parent->parent->y;
	}
	else
	{
		result.x = (globalX - x) + parent->x;
		result.y = (globalY - y) + parent->y;
	}
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
	case AnchorType::none:
		targetAnchors.left = AnchorType::none;
		targetAnchors.top = AnchorType::none;
		targetAnchors.right = AnchorType::none;
		targetAnchors.bottom = AnchorType::none;
		targetAnchors.horizontalCenter = AnchorType::none;
		targetAnchors.verticalCenter = AnchorType::none;
		targetAnchors.fill = AnchorType::none;
	}
	setDirty();
}

void Control::setAnchorMargin(AnchorType anchorType, float margin)
{
	switch (anchorType)
	{
	case AnchorType::left:
		margins.left = margin;
		break;
	case AnchorType::top:
		margins.top = margin;
		break;
	case AnchorType::right:
		margins.right = margin;
		break;
	case AnchorType::bottom:
		margins.bottom = margin;
		break;
	}
}

void Control::setAnchorMargin(float margin)
{
	margins.left = margin;
	margins.top = margin;
	margins.right = margin;
	margins.bottom = margin;
}

void Control::update()
{
	if (is_dirty || resource.window->is_resizing)
	{
		if (targetAnchors.fill != AnchorType::fill)
		{
			if (targetAnchors.left != AnchorType::none)
				x = parent->anchors[targetAnchors.left] + margins.left;
			else
				x = parent->x + localX;
		
			if (targetAnchors.top != AnchorType::none)
				y = parent->anchors[targetAnchors.top] + margins.top;
			else
				y = parent->y + localY;
		
			if (targetAnchors.right != AnchorType::none)
			{
				if (targetAnchors.left != AnchorType::none) // if the opposite anchor is set, then stretch the control rather than moving them
				{
					width = parent->width - (margins.left + margins.right);
				}
				else
				{
					x = (parent->anchors[targetAnchors.right] - width) - margins.right;
				}
				invokeSignal("resize");
			}
	
			if (targetAnchors.bottom != AnchorType::none) // same here but for top and bottom
			{
				if (targetAnchors.top != AnchorType::none)
				{
					height = parent->height - (margins.top + margins.bottom);
				}
				else
				{
					y = (parent->anchors[targetAnchors.bottom] - height) - margins.bottom;
				}
				invokeSignal("resize");
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
			this->x = (parent->x + localX) + margins.left;
			this->y = (parent->y + localY) + margins.top;
			width = parent->width - (margins.right + margins.left);
			height = parent->height - (margins.bottom + margins.top);
		}
	
		anchors[AnchorType::left] = x;
		anchors[AnchorType::top] = y;
		anchors[AnchorType::right] = x + width;
		anchors[AnchorType::bottom] = y + height;
		anchors[AnchorType::horizontalCenter] = (anchors[AnchorType::right] - (width / 2));
		anchors[AnchorType::verticalCenter] = anchors[AnchorType::bottom] - (height / 2);

		is_dirty = false;
		invokeSignal("update");
		invokeSignal("update_rect");
	}
}

void Control::draw()
{
	// empty function to be overriden
}