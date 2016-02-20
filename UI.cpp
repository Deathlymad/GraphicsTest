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
	keyBinds.addKeyBind(0, [this](unsigned short, KeyMap::KeyState) { deactivate(); if (_parent)_parent->activate(); }, "Quit", KeyMap::KeyState::ONPRESS);
	keyBinds.activate();
}

void UI::add(UI * ui)
{
	if (ui->_parent == nullptr) {
		ui->_parent = this;
		ui->_owner = _owner;
	}
	_children.push_back(ui);
}

void UI::add(UIPart * part)
{
	_parts.push_back(part);
}

void UI::init()
{
	renderer->build();
	for (UIPart* part : _parts)
		part->init();
	for (UI* child : _children)
		child->init();
}

void UI::load(RessourceHandler *loader)
{
	if (!renderer) {
		renderer = new Shader("assets/shaders/UIRender_vs.glsl", "assets/shaders/UIRender_fs.glsl");
		renderer->load(loader);
	}
	for (UIPart* part : _parts)
		part->load(loader);
	for (UI* child : _children)
		child->load(loader);
}

void UI::render()
{
	if (_enabled)
	{
		//internal render
		for (unsigned int i = _parts.size(); i > 0; i--)
		{
			renderer->bind();
			_parts[i-1]->render();
		}
		//external Render
		for (UI* child : _children)
			child->render();
	}
}

void UI::update()
{
	if (_enabled)
	{
		for (UIPart* part : _parts)
			part->update();
		for (UI* child : _children)
			child->update();
	}
}

UI & UI::operator=(UI & other)
{
	keyBinds = other.keyBinds;

	_parent = other._parent;
	_owner = other._owner;
	_children = other._children;
	other._children.~vector();
	_parts = other._parts;
	other._parts.~vector();
	_enabled = other._enabled;
	return *this;
}

UI::~UI()
{
}

vec2 UI::getScreenSize()
{
	return _parent ? _parent->getScreenSize() : _owner->getScreenSize();
}

void UI::addEvent(unsigned short key, function<void(unsigned short, KeyMap::KeyState)> Func, string name, int trig)
{
	keyBinds.addKeyBind(key, Func, name, trig);
}
