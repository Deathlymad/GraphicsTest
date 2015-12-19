#include "TestGame.h"



TestGame::TestGame() : Game(),
pt(glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, Attenuation(6.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f)),
Dir(glm::vec3(0.1f, 0.6f, 0.8f), 1.0f, glm::vec3(0.0f, 0.0f, 1.0f)),
m(32, 2),
obj("assets/mesh/untitled.obj", "assets/textures/tex1.bmp")
{
	pt.createUniforms("pLight");
	Dir.createUniforms("Light");

	//Rendering Registry
	getEngine()->getGraphicEngine()->registerGraphicObject(&pt);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Dir);

	getEngine()->getGraphicEngine()->initOnShaders([this](Shader*s) {m.init(s); });

	//scene Registry
	world.addObj(&pt);
	world.addObj(&Dir);

	Dir.add(&m);
	pt.add(&m);
	m.add(&obj);
}

TestGame::~TestGame()
{
}
