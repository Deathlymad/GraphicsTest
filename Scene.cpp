#include "Scene.h"
#include "RenderingEngine.h"
#include <GL\glew.h>


Scene::Scene() : root(), View()
{
}


void Scene::init(RenderingEngine* r, KeyMap* k)
{
	r->registerGraphicObject(&View);
	View.registerKeyBinds(k);
	root.init(r, k);
}

void Scene::init(RenderingEngine * r)
{
	r->registerGraphicObject(&View);
	root.init(r, nullptr);
}

void Scene::init(KeyMap * k)
{
	View.registerKeyBinds(k);
	root.init(nullptr, k);
}

void Scene::update()
{
	View.update();
	root.update();
}

void Scene::render(Shader* s)
{
	root.render(s);
}

Scene::~Scene()
{
}
