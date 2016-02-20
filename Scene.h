#include "EngineObject.h"
#include "Camera.h"
#include "UI.h"

class Shader;
class RessourceHandler;

#pragma once
class Scene
{
public:
	Scene(Game*);

	void addObj(EngineObject* obj)
	{
		_root.add(obj);
	}
	void addObj(UI* child)
	{
		_menu.add(child);
	}
	void addObj(UIPart* part)
	{
		_menu.add(part);
	}
	void setRootUI(UI& ui)
	{
		_menu = ui;
	}

	void load(RessourceHandler*);
	void init(KeyMap*);
	void update();
	void render(Shader*, bool, bool);

	bool getMenuState() { return _menu.isActive(); }

	void toggleMenu()
	{
		if (_menu.isActive())
			_menu.deactivate();
		else
			_menu.activate();
	}

	UI* getMenu() { return &_menu; }

	~Scene();
private:
	EngineObject _root;
	Camera _view;
	UI _menu;
};

