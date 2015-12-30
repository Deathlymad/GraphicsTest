#include "Game.h"
#include "Screen.h"
#include "RenderingEngine.h"


Game::Game() : screen(1366, 768, "Test", char(154)), Engine(&screen, this), world(Engine.getGraphicEngine())
{
	screen.handleWindow(InputHandler::registerCallbacks);
	KeyMaps.push_back(new KeyMap(&screen));

	setupKeyMap(*KeyMaps[0]);
	running = false;
}

void Game::Start()
{
	Engine.start();
	running = true;
	isMenuOpen = false;
	Run();
}

void Game::Run()
{
	bool is3D = true;
	while (running)
	{
		if (isMenuOpen)
		{
			if (!is3D)
			{
				is3D = true;
				Engine.getGraphicEngine()->set3D();
			}
			Engine.getGraphicEngine()->render(&world);
		}
		else
		{
			if (is3D)
			{
				is3D = false;
				Engine.getGraphicEngine()->set2D();
			}
			Engine.getGraphicEngine()->render(&Menu);
		}
	}
}

void Game::Terminate()
{
	running = false;
}

void Game::addObject(EngineObject & object)
{
	world.addObj(&object);
}

void Game::init()
{
	if (KeyMaps.size() > 0)
		KeyMaps[0]->launchKeyMap();
}

void Game::update()
{
	if (!isMenuOpen)
		world.update();
	else
		Menu.update();
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
	k.addKeyBind(0, [this](unsigned short) { ToggleMenu(); }, "Shutdown");
	world.init( &k);
}

void Game::ToggleMenu()
{
	isMenuOpen = !isMenuOpen;
}
