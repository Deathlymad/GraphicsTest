#include "Game.h"
#include "Screen.h"
#include "RenderingEngine.h"


Game::Game() : _screen(1366, 768, "Test", char(154)), _engine(&_screen, this), _world()
{
	_screen.handleWindow(InputHandler::registerCallbacks); //works only once i think
	KeyMaps.push_back(new KeyMap(&_screen));

	running = false;
	_menu = nullptr;
}

void Game::Start()
{
	_world.load(&_ressourceLoader);
	while (_ressourceLoader.loading()) {}
	_menu->init();
	setupKeyMap(*KeyMaps[0]);
	_engine.start();
	running = true;
	Run();
}

void Game::Run()
{
	bool is3D = true;
	while (running)
	{
		if (!_menu->isActive())
		{
			if (!is3D)
			{
				is3D = true;
				_menu->deactivate();
				_screen.disableCursor();
				KeyMaps[0]->activate();
				_engine.getGraphicEngine()->set3D();
			}
			_engine.getGraphicEngine()->render(&_world);
		}
		else
		{
			if (is3D)
			{
				is3D = false;
				_screen.enableCursor();
				KeyMaps[0]->deactivate();
				_engine.getGraphicEngine()->set2D();
			}
			_engine.getGraphicEngine()->render(_menu);
		}
	}
}

void Game::Terminate()
{
	running = false;
}

void Game::addObject(EngineObject & object)
{
	_world.addObj(&object);
}

void Game::update()
{
	if (!_menu->isActive())
		_world.update();
	else if (_menu)
		_menu->update();
}

KeyMap & Game::addKeyMap()
{
	unsigned int temp = KeyMaps.size();
	KeyMaps.insert( KeyMaps.begin() + temp, new KeyMap());
	setupKeyMap(*KeyMaps[temp]);
	return *KeyMaps[temp];
}

Game::~Game()
{
}

void Game::toggleMenu()
{
	if (_menu->isActive())
		_menu->deactivate();
	else
		_menu->activate();
}

void Game::setupKeyMap(KeyMap &k)
{
	k.addKeyBind(0, [this](unsigned short, KeyMap::KeyState)
	{
		if (!_menu->isActive())
			_menu->activate();
	}, "Menu", KeyMap::KeyState::ONRELEASE, 0);
	_world.init( &k);
}