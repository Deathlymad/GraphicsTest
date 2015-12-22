#include "CoreEngine.h"
#include "RenderingEngine.h"
#include "Game.h"

CoreEngine::CoreEngine(Screen* screen, Game* g) : _game(g), GrEngine( this, screen), UpdateThread([this] { init(); }, [this] { update(); }, 30)
{
}

void CoreEngine::start()
{
	UpdateThread.run();
}

RenderingEngine* CoreEngine::getGraphicEngine()
{
	return &GrEngine;
}

CoreEngine::~CoreEngine()
{
	UpdateThread.shutdown();
}

void CoreEngine::init()
{
	_game->init();
}

void CoreEngine::update()
{
	_game->update();
}
