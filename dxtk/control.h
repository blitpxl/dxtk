#pragma once
#include "unicode.h"
#include <unordered_map>
#include <d2d1.h>
#include <dwrite.h>
#include <string>

#include "dxobject.h"
#include "shitlog.h"
#include "d2dalias.h"

// declaration for SharedResource
class Window;

struct Point
{
	float x;
	float y;
};

struct SharedResource
{
	ID2D1HwndRenderTarget* renderTarget;
	IDWriteFactory* dwriteFactory;
	Window* window;
};

enum class AnchorType
{
	left,
	top,
	right,
	bottom,
	horizontalCenter,
	verticalCenter,
	center,
	fill,
	none
};

struct TargetAnchors
{
	AnchorType left				= AnchorType::none;
	AnchorType top				= AnchorType::none;
	AnchorType right			= AnchorType::none;
	AnchorType bottom			= AnchorType::none;
	AnchorType horizontalCenter	= AnchorType::none;
	AnchorType verticalCenter	= AnchorType::none;
	AnchorType fill				= AnchorType::none;
};

class Control : public DxObject
{
protected:
	TargetAnchors targetAnchors;
	std::wstring strbuf;
public:
	static inline SharedResource resource;
	float x;
	float y;
	float localX;
	float localY;
	float width;
	float height;
	float anchorPadding;
	bool is_window = false;
	bool is_debug = false;
	bool is_drawable = false;
	bool is_dirty = true;
	Control* parent;
	std::unordered_map<AnchorType, float> anchors;

	Control();
	Control(Control* parent);

	void setName(std::string const& name);
	void setAnchor(AnchorType controlAnchor, AnchorType targetParentAnchor = AnchorType::none);
	void setAnchorPadding(float padding);
	void requestRedraw();
	void setDirty();
	LPCWSTR toWString(std::string const& string);
	Point mapToLocal(float globalX, float globalY);
	virtual void move(float x, float y);
	virtual void setX(float x);
	virtual void setY(float y);
	virtual void resize(float width, float height);
	virtual void setWidth(float width);
	virtual void setHeight(float height);
	virtual void update();
	virtual void draw();
};