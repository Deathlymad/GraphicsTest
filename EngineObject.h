#include <vector>
#include "Def.h"
#include "RenderingEngine.h"

NSP_STD

class Shader;
class RenderingEngine;
class KeyMap;
class RessourceHandler;

#pragma once
class EngineObject
{
public:
	EngineObject();

	void add(EngineObject* obj) { if (obj && obj != this) children.push_back(obj); }

	virtual void load(RessourceHandler& loader);
	virtual void init( KeyMap& k); //check that initialization is only called once
	virtual void update(ThreadManager&);
	virtual void render(Shader&, RenderingEngine::RenderState);

	~EngineObject();
private:
	vector<EngineObject*> children;
};