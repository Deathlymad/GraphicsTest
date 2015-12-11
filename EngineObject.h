#include <vector>

class Shader;
class RenderingEngine;
class KeyMap;

#pragma once
class EngineObject
{
public:
	EngineObject();

	void add(EngineObject* obj) { children.push_back(obj); }

	virtual void init(RenderingEngine* r, KeyMap* k);
	virtual void update();
	virtual void render(Shader*);

	~EngineObject();
private:
	std::vector<EngineObject*> children;
};