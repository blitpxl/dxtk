#pragma once
#include <unordered_map>
#include <dxobject.h>
#include <d2d1.h>
#include <dwrite.h>
#include <shitlog.h>
#include <d2dalias.h>

// declaration for SharedResource
class Window;

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
	bool is_window = false;
	TargetAnchors targetAnchors;

public:
	static inline SharedResource resource;
	float x;
	float y;
	float localX;
	float localY;
	float width;
	float height;
	Control* parent;
	std::unordered_map<AnchorType, float> anchors;
	bool clearBeforeDraw = false;

	bool is_debug = false;
	bool redrawRequested = true;

	Control();
	Control(Control* parent);
	void setAnchor(AnchorType controlAnchor, AnchorType targetParentAnchor = AnchorType::none);
	void move(float x, float y);
	void requestRedraw();
	virtual void resize(float width, float height);
	virtual void update();
};