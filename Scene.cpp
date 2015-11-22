#include "Scene.h"
#include <GL\glew.h>


Scene::Scene() : root(), View()
{
}


void Scene::init(Shader* s, KeyMap* k)
{
	View.registerUniforms(s);
	View.registerKeyBinds(k);
}

void Scene::init(Shader * s)
{
	View.registerUniforms(s);
}

void Scene::init(KeyMap * k)
{
	View.registerKeyBinds(k);
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
