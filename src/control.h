#pragma once
#include <unordered_map>
#include <dxobject.h>
#include <d2d1.h>
#include <shitlog.h>

struct SharedResource
{
	ID2D1HwndRenderTarget* renderTarget;
};

enum class AnchorType
{
	left,
	top,
	right,
	bottom,
	horizontalCenter,
	verticalCenter,
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
};

class Control : public DxObject
{
protected:
	bool is_child = false;
	bool is_window = false;
	TargetAnchors targetAnchors;
public:
	float x;
	float y;
	float localX;
	float localY;
	float width;
	float height;
	Control* parent;
	std::unordered_map<AnchorType, float> anchors;
	SharedResource resource;

	bool is_debug = false;

	Control();
	Control(Control* parent);

	void setAnchor(AnchorType controlAnchor, AnchorType targetParentAnchor);
	void move(float x, float y);
	virtual void resize(float width, float height);
	virtual void update();
};