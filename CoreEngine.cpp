#include "CoreEngine.h"
#include "RenderingEngine.h"
#include "Game.h"

CoreEngine::CoreEngine(Screen* screen, Game* g) : _game(g), GrEngine( this, screen), lk(), UpdateThread([this] { init(); }, [this] { update(); }, 30)
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

std::mutex* CoreEngine::getLock()
{
	return &lk;
}

CoreEngine::~CoreEngine()
{
	UpdateThread.shutdown();
}

void CoreEngine::init()
{
	std::lock_guard<std::mutex> t(lk);
	_game->init();
}

void CoreEngine::update()
{
	std::lock_guard<std::mutex> t(lk);
	_game->update();
}
