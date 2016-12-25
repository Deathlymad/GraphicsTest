#include <vector>
#include "CoreEngine.h"
#include "Scene.h"
#include "Screen.h"

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

	virtual void load(RessourceHandler&);
	virtual void update(ThreadManager&);

	void load() { _engine.load(); }

	~Game();
private: //context
	Screen _screen;
	CoreEngine _engine;

protected:
	Scene& getScene() { return _world; }
	CoreEngine* getEngine() { return &_engine; }
private:
	Scene _world;

	bool running;
};

