#include <functional>

#include "ExclusiveObject.h"
#include "Light.h"
#include "Scene.h"
#include "Screen.h"
#include "Shader.h"

class Camera;
class CoreEngine;

#pragma once
class RenderingEngine : ThreadExclusiveObject<RenderingEngine>
{
public:
	RenderingEngine() : ThreadExclusiveObject<RenderingEngine>(*this) {}
	RenderingEngine(CoreEngine* parent, Screen*);

	void initOnShaders(std::function<void(Shader*)> f) { f(&ambient); for (BaseLight* l : Lights) f(l->getShader()); }

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
	std::vector<BaseLight*> Lights;


	CoreEngine* _parent;
};