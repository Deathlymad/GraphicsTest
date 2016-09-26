#include "Game.h"
#include "Screen.h"
#include "RenderingEngine.h"


Game::Game() : _screen(1366, 768, string("Test"), char(154)), _engine(&_screen, this), _world()
{
	_screen.handleWindow(InputHandler::registerCallbacks);
	KeyMaps.push_back(new KeyMap(&_screen));
	running = false;
}

void Game::Start()
{
	_world.init(KeyMaps[0]);
	_engine.start();
	running = true;
	Run();
}

void Game::Run()
{
	system_clock::time_point lastTick = system_clock::now();
	while (running)
	{
		running = !_screen.isScreenClosed();
		_engine.getGraphicEngine()->render(&_world);
		for (KeyMap* k : KeyMaps) KeyMap::updateKeyMap(k); //pulls Key updates if button is pressed
		auto passedTime = duration_cast<chrono::milliseconds>(system_clock::now() - lastTick);
		if (passedTime.count() < 16)
			this_thread::sleep_for(milliseconds(16) - passedTime);
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

void Game::update(ThreadManager* mgr)
{
	_world.update( mgr);
	KeyMaps[0]->updateKeyMap();
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