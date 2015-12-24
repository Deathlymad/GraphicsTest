#include "Scene.h"
#include "RenderingEngine.h"
#include <GL\glew.h>


Scene::Scene(RenderingEngine* r) : root(), View()
{
	r->registerGraphicObject(&View);
}

void Scene::init(KeyMap * k)
{
	View.registerKeyBinds(k);
	root.init( k);
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
