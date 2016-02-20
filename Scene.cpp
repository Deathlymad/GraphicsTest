#include "Scene.h"
#include "RenderingEngine.h"
#include <GL\glew.h>


Scene::Scene(Game* g) : _root(), _view(), _menu(g)
{}

void Scene::init(KeyMap * k)
{
	_view.registerKeyBinds(k);
	_root.init( k);
	_menu.init();
}

void Scene::load(RessourceHandler * loader)
{
	_root.load(loader);
	_menu.load(loader);
}

void Scene::update()
{
	if (_menu.isActive())
		_menu.update();
	else
	{
		_view.update();
		_root.update();
	}
}

void Scene::render(Shader* s, bool firstPass, bool is3D)
{
	if (is3D)
	{
		_view.render(s, firstPass);
		_root.render(s, firstPass);
	}
	else
		_menu.render();
}

Scene::~Scene()
{
}
