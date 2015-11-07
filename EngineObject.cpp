#include "EngineObject.h"



EngineObject::EngineObject()
{
}

void EngineObject::update()
{
	for (size_t i = 0; i < children.size(); i++)
		children[i]->update();
}

void EngineObject::render(Shader* s)
{
	for (size_t i = 0; i < children.size(); i++)
		children[i]->render(s);
}


EngineObject::~EngineObject()
{
}
