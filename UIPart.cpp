#include "UIPart.h"
#include "UI.h"
#include <GLFW\glfw3.h>


UIPart::UIPart(UI * parent, vec2 center, float range, function<void()> e) : _tex( Image("assets/textures/Test_tex2.bmp"))
{
	_parent = parent;

	_pos[0] = center;
	_pos[0].x -= range;
	_pos[0].y -= range;
	_pos[1] = center;
	_pos[1].x -= range;
	_pos[1].y -= range;

	vector<Mesh::Vertex> v;
	v.push_back(Mesh::Vertex(vec3(_pos[0].x, _pos[0].y, 0), vec2( 0.0f,  0.0f)));
	v.push_back(Mesh::Vertex(vec3(_pos[1].x, _pos[0].y, 0), vec2( 1.0f,  0.0f)));
	v.push_back(Mesh::Vertex(vec3(_pos[0].x, _pos[1].y, 0), vec2( 0.0f,  1.0f)));
	v.push_back(Mesh::Vertex(vec3(_pos[1].x, _pos[1].y, 0), vec2( 1.0f,  1.0f)));
	_mesh = Mesh(v, Mesh::VertexArrayObject::genBitset(2, 2));
	
	_event = e;

	_tex.glDownload();
}

UIPart::UIPart(UI * parent, vec2 pos1, vec2 pos2, function<void()> e) : _tex(Image("assets/textures/Test_tex2.bmp"))
{
	_parent = parent;
	_pos[0] = pos1;
	_pos[1] = pos2;

	vector<Mesh::Vertex> v;
	v.push_back(Mesh::Vertex(vec3(_pos[0].x, _pos[0].y, 0), vec2( 0.0f, 0.0f)));
	v.push_back(Mesh::Vertex(vec3(_pos[1].x, _pos[0].y, 0), vec2( 1.0f, 0.0f)));
	v.push_back(Mesh::Vertex(vec3(_pos[0].x, _pos[1].y, 0), vec2( 0.0f, 1.0f)));
	v.push_back(Mesh::Vertex(vec3(_pos[1].x, _pos[1].y, 0), vec2( 1.0f, 1.0f)));
	_mesh = Mesh(v, Mesh::VertexArrayObject::genBitset(2, 2));

	_event = e;

	_tex.glDownload();
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
	double convX = convertToScreenSpace( x, false);
	double convY = convertToScreenSpace( y, true);
	if ((_pos[0].x < convX && convX < _pos[1].x) && (_pos[0].y < convY && convY < _pos[1].y))
		return true;
	return false;
}

double UIPart::convertToScreenSpace(double in, bool height)
{
	return ((in/(height ? _parent->getScreenSize().y : _parent->getScreenSize().x)) * 2) - 1;
}

vec2 UIPart::convertToScreenSpace(vec2 in)
{
	return vec2(convertToScreenSpace(in.x, false), convertToScreenSpace(in.y, true));
}

UIButton::UIButton(UI* parent, vec2 pos1, vec2 pos2, unsigned short shortcut, function<void()> _event) : UIPart(parent, pos1, pos2, _event)
{
	if (shortcut != -1)
		_parent->addEvent(shortcut, [this](unsigned short, KeyMap::KeyState) {_pressed = !_pressed; }, "Shortcut", KeyMap::KeyState::ONRELEASE);
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