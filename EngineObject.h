#include <vector>
#include "Def.h"
#include "RessourceHandler.h"

NSP_STD

class Shader;
class RenderingEngine;
class KeyMap;

#pragma once
class EngineObject
{
public:
	EngineObject();

	void add(EngineObject* obj) { if (obj && obj != this) children.push_back(obj); }

	virtual void load(RessourceHandler* loader);
	virtual void init( KeyMap* k);
	virtual void update();
	virtual void render(Shader*, bool);

	~EngineObject();
private:
	vector<EngineObject*> children;
};