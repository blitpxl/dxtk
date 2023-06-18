#include <button.h>

Button::Button(Control* parent, float x, float y, float width, float height)
: Rect(parent, x, y, width, height), MouseArea(parent, x, y, width, height)
{

}