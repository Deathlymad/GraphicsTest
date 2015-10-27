#include "Scene.h"



Scene::Scene()
{
}


void Scene::init(Shader* s, KeyMap* k)
{
	View.registerUniforms(s);
	View.registerKeyBinds(k);
}

void Scene::update(Shader*)
{
	View.update();
	root.update();
}

void Scene::render(Shader*)
{
	root.render();
}

Scene::~Scene()
{
}
