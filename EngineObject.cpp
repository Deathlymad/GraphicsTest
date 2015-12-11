#include "EngineObject.h"
#include "RenderingEngine.h"
#include "KeyMap.h"


EngineObject::EngineObject() : children()
{}

void EngineObject::init(RenderingEngine* r, KeyMap* k)
{
	for (EngineObject* child : children)
		child->init(r, k);
}

void EngineObject::update()
{
	for (EngineObject* child : children)
		child->update();
}

void EngineObject::render(Shader* s)
{
	for (EngineObject* child : children)
		child->render(s);
}


EngineObject::~EngineObject()
{
}
