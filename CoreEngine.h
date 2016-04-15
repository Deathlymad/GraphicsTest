#include <mutex>
#include "RenderingEngine.h"
#include "RessourceHandler.h"
#include "Clock.h"

class Screen;
class Game;

#pragma once
class CoreEngine
{
public:
	CoreEngine(Screen*, Game*);

	void load();
	void start();

	RenderingEngine* getGraphicEngine();

	~CoreEngine();
private:
	void update();

	RessourceHandler _ressourceLoader;
	RenderingEngine GrEngine;
	Clock UpdateThread;

	Game* _game;
};