#include <vector>
#include "CoreEngine.h"
#include "Scene.h"
#include "KeyMap.h"
#include "UI.h"
#include "Screen.h"
#include "RessourceHandler.h"

#pragma once
class Game
{
public:
	Game();

	void Start();
	void Run();
	void Terminate();

	vec2 getScreenSize() { return vec2(_screen.getWidth(), _screen.getHeight()); }

	void addObject(EngineObject&);

	virtual void update();

	void toggleMenu();

	KeyMap& addKeyMap();

	~Game();
private: //context
	Screen _screen;
	CoreEngine _engine;

protected:
	RessourceLoader _ressourceLoader;
	Scene _world;
	CoreEngine* getEngine() { return &_engine; }
	void setMenu(UI* ui) { _menu = ui; _menu->load(&_ressourceLoader); }
private:

	UI* _menu;

	void setupKeyMap(KeyMap&);
	vector<KeyMap*> KeyMaps;

	bool running;
};

