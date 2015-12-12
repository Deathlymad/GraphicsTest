#include "EngineObject.h"
#include "RenderingEngine.h"
#include "KeyMap.h"


EngineObject::EngineObject() : children()
{}

void EngineObject::init(RenderingEngine* r, KeyMap* k)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->init(r, k);
}

void EngineObject::update()
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->update();
}

void EngineObject::render(Shader* s)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->render(s);
}


EngineObject::~EngineObject()
{
}
