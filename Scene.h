#include "EngineObject.h"
#include "Camera.h"

class Shader;

#pragma once
class Scene
{
public:
	Scene();

	void addObj(EngineObject* obj)
	{
		root.add(obj); //could lead to class slicing
	}

	void init(Shader*, KeyMap*);
	void init(Shader*);
	void init(KeyMap*);
	void update();
	void render(Shader*);

	~Scene();
private:
	EngineObject root;
	Camera View;
};

