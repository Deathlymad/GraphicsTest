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

	vec2 getScreenSize() { return vec2(screen.getWidth(), screen.getHeight()); }

	void addObject(EngineObject&);

	virtual void update();

	void toggleMenu();

	KeyMap& addKeyMap();

	~Game();
private: //context
	Screen screen;
	CoreEngine Engine;

protected:

	Scene world;
	UI* Menu;
	CoreEngine* getEngine() { return &Engine; }
private:
	void setupKeyMap(KeyMap&);
	vector<KeyMap*> KeyMaps;

	bool running;
};

