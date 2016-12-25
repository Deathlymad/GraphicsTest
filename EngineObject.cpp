#include "EngineObject.h"
#include "RenderingEngine.h"


EngineObject::EngineObject() : children()
{}

void EngineObject::init( Screen& k)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->init( k);
}

void EngineObject::load(RessourceHandler& loader)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->load(loader);
}

void EngineObject::update(ThreadManager& mgr)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->update(mgr);
}

void EngineObject::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	for (unsigned int i = 0; i < children.size(); i++)
		children[i]->render(s, firstPass);
}


EngineObject::~EngineObject()
{
}
