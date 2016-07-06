#include <mutex>
#include "RenderingEngine.h"
#include "RessourceHandler.h"
#include "Clock.h"
#include "ThreadManager.h"

class Screen;
class Game;

#pragma once
class CoreEngine : ThreadManager::CallableObject
{
public:
	CoreEngine(Screen*, Game*);

	void load();
	void start();

	virtual int run();

	ThreadManager* getThreadManager();
	RenderingEngine* getGraphicEngine();

	~CoreEngine();
private:

	RessourceHandler _ressourceLoader;
	RenderingEngine GrEngine;
	ThreadManager _manager;

	Game* _game;
};