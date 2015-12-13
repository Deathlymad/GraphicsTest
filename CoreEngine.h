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
	std::mutex* getLock();

	~CoreEngine();
private:
	void init();
	void update();

	RenderingEngine GrEngine;
	Clock UpdateThread;

	std::mutex lk;
	Game* _game;
};