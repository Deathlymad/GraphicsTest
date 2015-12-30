#include "UIPart.h"
#include "UI.h"
#include <GLFW\glfw3.h>


UIPart::UIPart(UI * parent, vec2 center, float range, function<void()> e)
{
	_parent = parent;

	_pos[0] = center;
	_pos[0].x -= range;
	_pos[0].y -= range;
	_pos[1] = center;
	_pos[1].x -= range;
	_pos[1].y -= range;

	_event = e;
}

UIPart::UIPart(UI * parent, vec2 pos1, vec2 pos2, function<void()> e)
{
	_parent = parent;
	_pos[0] = pos1;
	_pos[1] = pos2;

	_event = e;
}

void UIPart::render()
{
	_tex.bind();
	_mesh.Draw();
}

UIPart::~UIPart()
{
}

bool UIPart::isInPart(double x, double y)
{
	if ((_pos[0].x < x < _pos[3].x) && (_pos[0].y < y < _pos[3].y))
		return true;
	return false;
}

void UIButton::update()
{
	if (_pressed)
		_event();
}

void UIButton::onMouseButton(char button, char action, char mods)
{
	double x, y;
	this->getCursorPos( x, y);
	if ( _parent->isActive() && isInPart(x, y) && button== GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			_pressed = true;
		else if (action == GLFW_RELEASE)
			_pressed = false;
	}
}