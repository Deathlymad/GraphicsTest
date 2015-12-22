#include <mutex>
#include "RenderingEngine.h"

class Screen;
class Game;

#pragma once
class CoreEngine
{
public:
	CoreEngine(Screen*, Game*);

	void start();

	RenderingEngine* getGraphicEngine();

	~CoreEngine();
private:
	void init();
	void update();

	RenderingEngine GrEngine;
	Clock UpdateThread;

	Game* _game;
};