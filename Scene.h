#include "EngineObject.h"
#include "Camera.h"

class Shader;

#pragma once
class Scene
{
public:
	Scene();

	void init(Shader*);
	void update(Shader*);
	void render(Shader*);

	~Scene();
private:
	EngineObject root;
	Camera View;
};

