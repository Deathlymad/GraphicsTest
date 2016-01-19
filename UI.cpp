#include "Game.h"
#include "UI.h"
#include "UIPart.h"


Shader* UI::renderer = nullptr;

UI::UI(Game* parent)
{
	_parent = nullptr;
	_owner = parent;
	_parts.push_back(new UIPart(this, vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f))); //add Background
	_enabled = true;
	keyBinds.activate();
}

UI::UI(UI* parent)
{
	_parent = parent;
	_owner = _parent->_owner;
	_parts.push_back(new UIPart(this, vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f))); //add Background
	_enabled = true;
	keyBinds.addKeyBind(0, [this](unsigned short, KeyMap::KeyState) { deactivate(); _parent->activate(); }, "Quit", KeyMap::KeyState::ONPRESS);
	keyBinds.activate();
}

void UI::add(UI * ui)
{
	_children.push_back(ui);
}

void UI::add(UIPart * part)
{
	_parts.push_back(part);
}

void UI::render()
{
	if (!renderer)
		renderer = new Shader("UIRender_vs.glsl", "UIRender_fs.glsl");
	if (_enabled)
	{
		//internal render
		renderer->bind();
		for (unsigned int i = _parts.size(); i > 0; i--)
			_parts[i-1]->render();
	}
	//external Render
	for (UI* child : _children)
		child->render();
}

void UI::update()
{
	if (_enabled)
		for (UIPart* part : _parts)
			part->update();
	//external Update
	for (UI* child : _children)
		child->update();
}


UI::~UI()
{
}

vec2 UI::getScreenSize()
{
	return _parent ? _parent->getScreenSize() : _owner->getScreenSize();
}

void UI::addEvent(unsigned short key, function<void(unsigned short, KeyMap::KeyState)> Func, string name, int trig, unsigned char priority)
{
	keyBinds.addKeyBind(key, Func, name, trig, priority);
}
