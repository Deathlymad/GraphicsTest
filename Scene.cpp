#include "Scene.h"
#include "RenderingEngine.h"
#include <GL\glew.h>


Scene::Scene() : root(), View()
{}

void Scene::init(KeyMap * k)
{
	View.registerKeyBinds(k);
	root.init( k);
}

void Scene::load(RessourceLoader * loader)
{
	root.load(loader);
}

void Scene::update()
{
	View.update();
	root.update();
}

void Scene::render(Shader* s, bool firstPass)
{
	View.render(s, firstPass);
	root.render(s, firstPass);
}

Scene::~Scene()
{
}
