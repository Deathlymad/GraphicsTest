#include "EngineObject.h"
#include "RenderingEngine.h"
#include "KeyMap.h"


EngineObject::EngineObject() : children()
{}

void EngineObject::init( KeyMap* k)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->init( k);
}

void EngineObject::load(RessourceHandler * loader)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->load(loader);
}

void EngineObject::update()
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->update();
}

void EngineObject::render(Shader* s, bool firstPass)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->render(s, firstPass);
}


EngineObject::~EngineObject()
{
}
