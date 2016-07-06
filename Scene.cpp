#include "Scene.h"
#include "RenderingEngine.h"
#include <GL\glew.h>


Scene::Scene() : _root(), _view()
{}

void Scene::init(KeyMap * k)
{
	_view.registerKeyBinds(k);
	_root.init( k);
}

void Scene::load(RessourceHandler * loader)
{
	_root.load(loader);
}

void Scene::update(ThreadManager* mgr)
{
	_view.update(mgr);
	_root.update(mgr);
}

void Scene::render(Shader* s, RenderingEngine::RenderState firstPass)
{
	_view.render(s, firstPass);
	_root.render(s, firstPass);
}

Scene::~Scene()
{
}
