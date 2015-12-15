#include "TestGame.h"



TestGame::TestGame() : Game()
{
	pt = PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, Attenuation(6.0f, 0.0f, 0.0f), glm::vec3( 1.0f, 1.0f, 1.0f));
	pt.writeUniform("pLight");

	Dir = DirectionalLight(glm::vec3(0.1f, 0.0f, 0.3f), 10.0f, glm::normalize(glm::vec3( 1.0f, 1.0f,  1.0f)));
	Dir.writeUniform("Light");
	obj = ModelRenderer("assets/mesh/Test_Block.obj", "assets/textures/tex1.bmp", getEngine()->getGraphicEngine());

	world.addObj(&pt);
	world.addObj(&Dir);
	world.addObj(&obj);
}

TestGame::~TestGame()
{
}
