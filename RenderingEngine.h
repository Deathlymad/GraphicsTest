#include <functional>
#include <vector>

#include "Def.h"
#include "Shader.h"

NSP_STD


class Camera;
class CoreEngine;
class Screen;
class Scene;
class Texture;
class UI;
class BaseLight;
class RessourceHandler;

#pragma once
class RenderingEngine
{
public:
	enum RenderState
	{
		AMBIENT_PASS,
		FWD_RENDER,
		POST_RENDER
	};

	RenderingEngine(CoreEngine* parent, Screen*);

	void load(RessourceHandler* loader);
	void init();

	void render(Scene*);
	
	void set3D();

	void registerGraphicObject(BaseLight*);

	RenderingEngine& operator= (RenderingEngine& engine)
	{
		_screen = engine._screen;
		ambient = engine.ambient;
		Lights = engine.Lights;
		_parent = engine._parent;
		return *this;
	}

	~RenderingEngine();
private:

	void setup3DEngineState();

	Screen* _screen;

	Shader ambient;
	vector<BaseLight*> Lights;

	CoreEngine* _parent;
};