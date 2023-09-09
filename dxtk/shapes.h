#include "renderer.h"

class Window;

class Shapes
{
	Renderer* renderer;
	ID2D1SolidColorBrush* brush;
public:
	Shapes(Window* window);
	void DrawRect(float x, float y, float width, float height, ColorType color);
	void DrawEllipse(float x, float y, float size, ColorType color);
};