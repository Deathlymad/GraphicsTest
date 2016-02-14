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

	void load(RessourceHandler* loader);
	void init();

	void render(Scene*);
	void render(UI*);

	void set2D();
	void set3D();

	void registerGraphicObject(BaseLight*);

	RenderingEngine& operator= (RenderingEngine& engine)
	{
		_screen = engine._screen;

		ambient = engine.ambient;

		Lights = engine.Lights;
		return *this;
	}

	~RenderingEngine();
private:
	void setup3DEngineState();
	void setup2DEngineState();

	Screen* _screen;

	Shader ambient;
	vector<BaseLight*> Lights;


	CoreEngine* _parent;
};