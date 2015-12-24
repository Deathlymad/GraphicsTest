#include "EngineObject.h"
#include "Camera.h"

class Shader;
class RenderingEngine;

#pragma once
class Scene
{
public:
	Scene(RenderingEngine*);

	void addObj(EngineObject* obj)
	{
		root.add(obj); //could lead to class slicing
	}

	void init(KeyMap*);
	void update();
	void render(Shader*, bool);

	~Scene();
private:
	EngineObject root;
	Camera View;
};

