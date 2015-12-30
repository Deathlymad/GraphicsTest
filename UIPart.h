#include <glm\glm.hpp>
#include "Def.h"
#include "InputHandler.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

NSP_STD
NSP_GLM

class UI;

#pragma once
class UIPart : public InputHandler
{
public:
	UIPart(UI* parent, vec2 center, float range, function<void()> e = []() {});
	UIPart(UI* parent, vec2 pos1, vec2 pos2, function<void()> e= []() {});

	virtual void render();

	~UIPart();
protected:
	bool isInPart(double x, double y);

	function<void()> _event;
	UI* _parent;
private:
	vec2 _pos[2]; //from bottom left, top right
	Mesh2D _mesh;
	Texture _tex;
};

class UIButton : UIPart
{
public:
	virtual void update();
	virtual void onMouseButton(char button, char action, char mods);
private:
	bool _pressed;
};

class UIFader : UIPart
{
public:
	virtual void render();
private:
	UIPart _fader;
};

class UIText : UIPart
{
private:
	static Shader textRendenerer;
};