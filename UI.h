#include "EngineObject.h"
#include "Shader.h"
#include "KeyMap.h"


class UIPart;
class Game;

#pragma once
class UI
{
	friend class UIPart;
	friend class UIButton;
public:
	UI(Game* parent);
	UI(UI* parent = nullptr);

	void add(UI*);
	void add(UIPart*);
	void addEvent(unsigned short, function<void(unsigned short, KeyMap::KeyState)>, string, int trig = KeyMap::KeyState::ONHOLD);

	bool isActive() { return _enabled; }

	void activate() { _enabled = true; keyBinds.activate(); }
	void deactivate() { _enabled = false; keyBinds.deactivate(); }

	virtual void load(RessourceHandler*);
	virtual void init();
	virtual void render();
	virtual void update();

	UI& operator=(UI& other);

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