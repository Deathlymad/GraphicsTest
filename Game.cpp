#include "Game.h"
#include "ModelRenderer.h"


Game::Game() : screen(1366, 768, "Test", char(154)), GraphicEngine(), UpdateThread([] {}, [this] {update(); }, 60)
{
	GraphicEngine = RenderingEngine(&screen);
	KeyMaps.push_back(KeyMap(&screen));
	setupKeyMap(KeyMaps[0]);
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
	
	KeyMaps[0].launchKeyMap();

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
	KeyMaps.insert( KeyMaps.begin() + temp, KeyMap());
	setupKeyMap(KeyMaps[temp]);
	return KeyMaps[temp];
}


Game::~Game()
{
}

void Game::buildWorld()
{
	DirectionalLight Dir = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1000.0f, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	Dir.writeUniform("Light");

	world.addObj( new ModelRenderer("assets/mesh/stein_einfach.obj", ""));
}

void Game::setupKeyMap(KeyMap &k)
{
	k.addKeyBind(0, [this](unsigned short) { Terminate(); }, "Shutdown");
	GraphicEngine.initOnShaders([this](Shader* s) {world.init(s); });
	world.init(&k);
}
