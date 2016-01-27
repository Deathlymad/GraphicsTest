#include <functional>

#include "ExclusiveObject.h"
#include "Light.h"
#include "Shader.h"

class Camera;
class CoreEngine;
class Screen;
class Scene;
class Texture;
class UI;

#pragma once
class RenderingEngine
{
public:
	RenderingEngine(CoreEngine* parent, Screen*);

	void init();

	void render(Scene*);
	void render(UI*);

	void set2D();
	void set3D();

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
	void setup3DEngineState();
	void setup2DEngineState();

	Screen* screen;

	Shader ambient;
	vector<BaseLight*> Lights;


	CoreEngine* _parent;
};