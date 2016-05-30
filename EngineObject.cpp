#include "EngineObject.h"
#include "RenderingEngine.h"
#include "KeyMap.h"


EngineObject::EngineObject() : children()
{}

void EngineObject::init( KeyMap* k)
{
	for (unsigned int i = 0; i < children.size(); i++)
		if (children[i])
			children[i]->init( k);
}

void EngineObject::load(RessourceHandler * loader)
{
	for (unsigned int i = 0; i < children.size(); i++)
		if (children[i])
			children[i]->load(loader);
}

void EngineObject::update()
{
	for (unsigned int i = 0; i < children.size(); i++)
		if (children[i])
			children[i]->update();
}

void EngineObject::render(Shader* s, RenderingEngine::RenderState firstPass)
{
	for (unsigned int i = 0; i < children.size(); i++)
		if (children[i])
			children[i]->render(s, firstPass);
}


EngineObject::~EngineObject()
{
}
