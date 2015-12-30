#include <vector>
#include "CoreEngine.h"
#include "Scene.h"
#include "KeyMap.h"
#include "UI.h"
#include "Screen.h"

#pragma once
class Game
{
public:
	Game();

	void Start();
	void Run();
	void Terminate();

	void addObject(EngineObject&);

	virtual void init();
	virtual void update();

	KeyMap& addKeyMap();

	~Game();
private: //context
	Screen screen;
	CoreEngine Engine;

protected:
	Scene world;
	UI Menu;
	bool isMenuOpen;
	CoreEngine* getEngine() { return &Engine; }
private:
	void setupKeyMap(KeyMap&);
	void ToggleMenu();
	vector<KeyMap*> KeyMaps;

	bool running;
};

