#include "EngineObject.h"

#pragma once
class Scene
{
public:
	Scene();
	~Scene();
private:
	EngineObject root;
	Camera View;
};

