#include "EngineObject.h"
#include "Shader.h"
#include "KeyMap.h"


class UIPart;
class Game;

#pragma once
class UI : EngineObject
{
	friend class UIPart;
	friend class UIButton;
public:
	UI(Game* parent);
	UI(UI* parent);

	void add(UI*);
	void add(UIPart*);

	bool isActive() { return _enabled; }

	void activate() { _enabled = true; keyBinds.activate(); }
	void deactivate() { _enabled = false; keyBinds.deactivate(); }

	void render();
	void update();

	~UI();
private:
	vec2 getScreenSize();
	void addEvent(unsigned short, function<void(unsigned short, KeyMap::KeyState)>, string, int);

	KeyMap keyBinds;
	UI* _parent;
	Game* _owner;
	vector<UI*> _children;
	vector<UIPart*> _parts;
	bool _enabled;

	static Shader* renderer;
};