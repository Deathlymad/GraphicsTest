#include <mutex>
#include "RenderingEngine.h"

class Screen;
class Game;

#pragma once
class CoreEngine
{
public:
	CoreEngine(Screen*, Game*);

	void graphicInit();
	void start();

	RenderingEngine* getGraphicEngine();
	mutex* getLock();

	~CoreEngine();
private:
	void init();
	void update();

	RenderingEngine GrEngine;
	Clock UpdateThread;

	mutex lk;
	Game* _game;
};