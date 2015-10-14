#include <vector>

#pragma once
class EngineObject
{
public:
	EngineObject();

	void update();
	void render();

	~EngineObject();
private:
	std::vector<EngineObject> children;
};

