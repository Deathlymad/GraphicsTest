#include "Game.h"
#include "Screen.h"
#include "RenderingEngine.h"


Game::Game() : screen(1366, 768, "Test", char(154)), Engine(&screen, this), world(Engine.getGraphicEngine())
{
	screen.handleWindow(InputHandler::registerCallbacks);
	KeyMaps.push_back(new KeyMap(&screen));

	setupKeyMap(*KeyMaps[0]);
	running = false;
	Menu = nullptr;
}

void Game::Start()
{
	Engine.start();
	running = true;
	Run();
}

void Game::Run()
{
	bool is3D = true;
	while (running)
	{
		if (!Menu->isActive())
		{
			if (!is3D)
			{
				is3D = true;
				Menu->deactivate();
				screen.disableCursor();
				KeyMaps[0]->activate();
				Engine.getGraphicEngine()->set3D();
			}
			Engine.getGraphicEngine()->render(&world);
		}
		else
		{
			if (is3D)
			{
				is3D = false;
				screen.enableCursor();
				KeyMaps[0]->deactivate();
				Engine.getGraphicEngine()->set2D();
			}
			Engine.getGraphicEngine()->render(Menu);
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
	KeyMaps[0]->activate();
}

void Game::update()
{
	if (!Menu->isActive())
		world.update();
	else if (Menu)
		Menu->update();
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
	if (Menu->isActive())
		Menu->deactivate();
	else
		Menu->activate();
}

void Game::setupKeyMap(KeyMap &k)
{
	k.addKeyBind(0, [this](unsigned short, KeyMap::KeyState)
	{
		toggleMenu();
	}, "Menu", KeyMap::KeyState::ONRELEASE);
	world.init( &k);
}