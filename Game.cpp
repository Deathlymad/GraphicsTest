#include "Game.h"


Game::Game() : screen(1366, 768, "Test", char(154)), GraphicEngine(), UpdateThread([] {}, [this] {update(); }, 60)
{
	GraphicEngine = RenderingEngine(&screen);
	screen.handleWindow(InputHandler::registerCallbacks);
	KeyMaps.push_back(new KeyMap(&screen));
	setupKeyMap(*KeyMaps[0]);
	running = false;
}

void Game::Start()
{
	running = true;
	Run();
}

void Game::Run()
{
	buildWorld();
	world.init(&GraphicEngine);

	KeyMaps[0]->launchKeyMap();

	while (running)
	{
		GraphicEngine.render(&world);
	}
	UpdateThread.shutdown();
}

void Game::Terminate()
{
	running = false;
}

void Game::addObject(EngineObject & object)
{
	world.addObj(&object);
}

void Game::update()
{
	world.update();
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

void Game::setupKeyMap(KeyMap &k)
{
	k.addKeyBind(0, [this](unsigned short) { Terminate(); }, "Shutdown");
	world.init( &k);
}
