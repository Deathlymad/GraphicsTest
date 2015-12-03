#include <vector>
#include "RenderingEngine.h"
#include "Scene.h"
#include "KeyMap.h"

#pragma once
class Game
{
public:
	Game();

	void Start();
	void Run();
	void Terminate();

	void addObject(EngineObject&);

	void update();

	KeyMap& addKeyMap();

	~Game();
private:
	virtual void buildWorld(); //kind of like post init here are all the world objects created
	void setupKeyMap(KeyMap&);

	std::vector<KeyMap*> KeyMaps;
	RenderingEngine GraphicEngine;
	Screen screen;
	Scene world; //needs to be extended and changed

	Clock UpdateThread;

	bool running;
};

