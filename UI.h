#include "EngineObject.h"
#include "Shader.h"
#include "KeyMap.h"


class UIPart;
class Game;

#pragma once
class UI : EngineObject
{
public:
	UI(Game* parent);
	UI(UI* parent);

	bool isActive() { return _enabled; }

	void activate() { _enabled = true; keyBinds.activate(); }
	void deactivate() { _enabled = false; keyBinds.deactivate(); }

	void render();
	void update();

	~UI();
private:
	KeyMap keyBinds;
	UI* _parent;
	Game* _owner;
	vector<UI*> _children;
	vector<UIPart*> _parts;
	bool _enabled;

	static Shader* renderer;
};

