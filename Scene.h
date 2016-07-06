#include "EngineObject.h"
#include "Camera.h"
#include "UI.h"

class Shader;
class RessourceHandler;

#pragma once
class Scene
{
public:
	Scene();

	void addObj(EngineObject* obj)
	{
		_root.add(obj);
	}
	
	void load(RessourceHandler*);
	void init(KeyMap*);
	void update(ThreadManager*);
	void render(Shader*, RenderingEngine::RenderState);

	Camera& getCam() { return _view; }

	~Scene();
private:
	EngineObject _root;
	Camera _view;
};

