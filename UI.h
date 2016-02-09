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
	void addEvent(unsigned short, function<void(unsigned short, KeyMap::KeyState)>, string, int trig = KeyMap::KeyState::ONHOLD, unsigned char priority = 0);

	bool isActive() { return _enabled; }

	void activate() { _enabled = true; keyBinds.activate(); }
	void deactivate() { _enabled = false; keyBinds.deactivate(); }

	void load(RessourceHandler*);
	void init();
	void render();
	void update();

	~UI();
private:
	vec2 getScreenSize();

	KeyMap keyBinds;
	UI* _parent;
	Game* _owner;
	vector<UI*> _children;
	vector<UIPart*> _parts;
	bool _enabled;

	static Shader* renderer;
};