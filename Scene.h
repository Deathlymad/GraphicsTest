#include "EngineObject.h"
#include "Camera.h"

class Shader;
class RessourceHandler;

#pragma once
class Scene
{
public:
	Scene();

	void addObj(EngineObject* obj)
	{
		root.add(obj); //could lead to class slicing
	}

	void load(RessourceHandler*);
	void init(KeyMap*);
	void update();
	void render(Shader*, bool);

	~Scene();
private:
	EngineObject root;
	Camera View;
};

