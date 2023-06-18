#pragma once
#include <dxobject.h>
#include <d2d1.h>
#include <shitlog.h>

struct SharedResource
{
	ID2D1HwndRenderTarget* renderTarget;
};

class Control : public DxObject
{
protected:
	bool is_child = false;
	bool is_window = false;
public:
	float x;
	float y;
	float localX;
	float localY;
	float width;
	float height;
	Control* parent;
	SharedResource resource;
	bool is_debug = false;

	Control();
	Control(Control* parent);

	void move(float x, float y);
	void resize(float width, float height);
	virtual void update();
};