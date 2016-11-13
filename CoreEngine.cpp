#include "CoreEngine.h"
#include "RenderingEngine.h"
#include "Game.h"

CoreEngine::CoreEngine(Screen* _screen, Game* g) : _game(g), GrEngine( this, _screen), _ressourceLoader(&_manager)
{
}

void CoreEngine::load()
{
	_game->load(_ressourceLoader);
	GrEngine.load(_ressourceLoader);
}

void CoreEngine::start()
{
	GrEngine.init();
	_manager.registerCallable(this);
}

ThreadManager * CoreEngine::getThreadManager()
{
	return &_manager;
}

RenderingEngine* CoreEngine::getGraphicEngine()
{
	return &GrEngine;
}

CoreEngine::~CoreEngine()
{
}

int CoreEngine::run()
{
	_game->update(_manager);
	return 0;
}
