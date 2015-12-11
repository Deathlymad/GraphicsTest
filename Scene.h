#include "EngineObject.h"
#include "Camera.h"

class Shader;
class RenderingEngine;

#pragma once
class Scene
{
public:
	Scene();

	void addObj(EngineObject* obj)
	{
		root.add(obj); //could lead to class slicing
	}

	void init(RenderingEngine*, KeyMap*);
	void init(RenderingEngine*);
	void init(KeyMap*);
	void update();
	void render(Shader*);

	~Scene();
private:
	EngineObject root;
	Camera View;
};

