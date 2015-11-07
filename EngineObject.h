#include <vector>

class Shader;
#pragma once
class EngineObject
{
public:
	EngineObject();

	void add(EngineObject* obj) { children.push_back(obj); }

	virtual void update();
	virtual void render(Shader*);

	~EngineObject();
private:
	std::vector<EngineObject*> children;
};