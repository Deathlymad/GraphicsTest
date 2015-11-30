#include <functional>

#include "ExclusiveObject.h"
#include "Light.h"
#include "Scene.h"
#include "Screen.h"
#include "Shader.h"

#pragma once
class RenderingEngine : ThreadExclusiveObject<RenderingEngine>
{
public:
	RenderingEngine() : ThreadExclusiveObject<RenderingEngine>(*this) {}
	RenderingEngine(Screen*);

	void initOnShaders(std::function<void(Shader*)> f) { f(&ambient); for (BaseLight* l : Lights) f(l->getShader()); }

	void render(Scene*);

	void registerGraphicObject(BaseLight*);

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

	Shader ambient;
	std::vector<BaseLight*> Lights;
};