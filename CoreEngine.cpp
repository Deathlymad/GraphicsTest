#include "CoreEngine.h"
#include "RenderingEngine.h"
#include "Game.h"

CoreEngine::CoreEngine(Screen* _screen, Game* g) : _game(g), GrEngine( this, _screen), UpdateThread([this] {}, [this] { update(); }, 30)
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

void CoreEngine::update()
{
	_game->update();
}
