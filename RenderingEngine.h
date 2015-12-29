#include <functional>

#include "ExclusiveObject.h"
#include "Light.h"
#include "Scene.h"
#include "Screen.h"
#include "Shader.h"

class Camera;
class CoreEngine;

#pragma once
class RenderingEngine
{
public:
	RenderingEngine(CoreEngine* parent, Screen*);

	void initOnShaders(function<void(Shader*, bool)> f) { f(&ambient, false); for (BaseLight* l : Lights) f(l->getShader(), true); }

	void render(Scene*);

	void registerGraphicObject(BaseLight*);
	void registerGraphicObject(Camera*);

	RenderingEngine& operator= (RenderingEngine& engine)
	{
		screen = engine.screen;

		ambient = engine.ambient;

		Lights = engine.Lights;
		return *this;
	}

	~RenderingEngine();
private:
	void setupInitialEngineState();

	Screen* screen;

	Camera* MainView; //needs way of showing several pictures (PIP, mirrors, etc.) without several renderingengines

	Shader ambient;
	vector<BaseLight*> Lights;


	CoreEngine* _parent;
};