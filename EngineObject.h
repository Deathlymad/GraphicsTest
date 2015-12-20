#include <vector>
#include "Def.h"

NSP_STD

class Shader;
class RenderingEngine;
class KeyMap;

#pragma once
class EngineObject
{
public:
	EngineObject();

	void add(EngineObject* obj) { children.push_back(obj); }

	virtual void init( KeyMap* k);
	virtual void update();
	virtual void render(Shader*);

	~EngineObject();
private:
	vector<EngineObject*> children;
};