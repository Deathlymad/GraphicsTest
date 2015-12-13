#include <vector>
#include "CoreEngine.h"
#include "Scene.h"
#include "KeyMap.h"

class Screen;

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
protected:
	Scene world;
private:
	void setupKeyMap(KeyMap&);

	std::vector<KeyMap*> KeyMaps;
	Screen screen;
	CoreEngine Engine;

	bool running;
};

