#include "Game.h"
#include "ModelRenderer.h"


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

void Game::buildWorld() //Lazy Implementation
{
	DirectionalLight* Dir = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1000.0f, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	
	GraphicEngine.registerGraphicObject(Dir);

	world.addObj( new ModelRenderer("assets/mesh/stein_einfach.obj", "assets/textures/tex1.bmp"));
}

void Game::setupKeyMap(KeyMap &k)
{
	k.addKeyBind(0, [this](unsigned short) { Terminate(); }, "Shutdown");
	GraphicEngine.initOnShaders([this](Shader* s) {world.init(s); });
	world.init(&k);
}
