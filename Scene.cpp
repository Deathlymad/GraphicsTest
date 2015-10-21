#include "Scene.h"



Scene::Scene()
{
}


void Scene::init(Shader* s)
{
	View.registerUniforms(s);
	
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
