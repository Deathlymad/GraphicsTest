#include "Game.h"
#include "Screen.h"
#include "RenderingEngine.h"


Game::Game() : _screen(1366, 768, "Test", char(154)), _engine(&_screen, this), _world(this)
{
	_screen.handleWindow(InputHandler::registerCallbacks);
	KeyMaps.push_back(new KeyMap(&_screen));

	running = false;
}

void Game::Start()
{
	_engine.start();
	_world.init(KeyMaps[0]);
	running = true;
	Run();
}

void Game::Run()
{
	bool is3D = true;
	unsigned int minLoopTime = 16;
	system_clock::time_point lastTick = system_clock::now();
	while (running)
	{
		if (!_world.getMenuState())
		{
			if (!is3D)
			{
				is3D = true;
				_screen.disableCursor();
				KeyMaps[0]->activate();
				_engine.getGraphicEngine()->set3D();
				minLoopTime = 16;
			}
		}
		else
		{
			if (is3D)
			{
				is3D = false;
				_screen.enableCursor();
				KeyMaps[0]->deactivate();
				_engine.getGraphicEngine()->set2D();
				minLoopTime = 50; //slow ticking
			}
		}
		_engine.getGraphicEngine()->render(&_world);
		this_thread::sleep_for(milliseconds(minLoopTime) - duration_cast<chrono::milliseconds>(system_clock::now() - lastTick));
		lastTick = system_clock::now();
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

void Game::load(RessourceHandler * loader)
{
	_world.load(loader);
}

void Game::update()
{
	_world.update();
}

KeyMap & Game::addKeyMap()
{
	unsigned int temp = KeyMaps.size();
	KeyMaps.insert( KeyMaps.begin() + temp, new KeyMap());
	return *KeyMaps[temp];
}

Game::~Game()
{
}