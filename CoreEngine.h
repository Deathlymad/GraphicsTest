#include <mutex>
#include "RenderingEngine.h"
#include "Clock.h"

class Screen;
class Game;

#pragma once
class CoreEngine
{
public:
	CoreEngine(Screen*, Game*);

	void load(RessourceHandler*);
	void start();

	RenderingEngine* getGraphicEngine();

	~CoreEngine();
private:
	void update();

	RenderingEngine GrEngine;
	Clock UpdateThread;

	Game* _game;
};