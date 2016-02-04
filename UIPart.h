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

	virtual void load(RessourceLoader*);
	virtual void init();
	virtual void render();
	virtual void update() {}

	~UIPart();
protected:
	bool isInPart(double x, double y);
	double convertToScreenSpace(double, bool);
	vec2 convertToScreenSpace(vec2);

	function<void()> _event;
	UI* _parent;
	Mesh _mesh;
	Texture* _tex;
private:
	vec2 _pos[2]; //from bottom left, top right
};

class UIButton : public UIPart
{
public:
	UIButton(UI* parent, vec2 pos1, vec2 pos2, unsigned short = -1, function<void()> = [] {});
	
	virtual void update();
	virtual void onMouseButton(char button, char action, char mods);
private:
	bool _pressed;
};

class UIFader : public UIPart
{
public:
	virtual void render();
private:
	UIPart _fader;
};

class UIText : public UIPart
{
public:
	UIText(UI* parent, vec2 pos1, vec2 pos2, function<void()> = [] {});
	virtual void render();
private:
	static Shader* textRendenerer;
	static TextureAtlas* glyphMap;
};