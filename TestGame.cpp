#include "TestGame.h"
#include "ModelRenderer.h"



TestGame::TestGame() : Game()
{
}

TestGame::~TestGame()
{
}

void TestGame::buildWorld()
{
	//, glm::normalize(glm::vec3(-1.0, 1.0, -1.0)), 10.0f
	spot = PointLight(glm::vec3(0.3, 0.0, 0.0), 10.0f, Attenuation(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f, 1.0f, 1.0f));
	world.addObj(&spot);
	spot.writeUniform("pLight");

	Dir = DirectionalLight(glm::vec3(0.1f, 0.0f, 0.3f), 1.0f, glm::normalize(glm::vec3( 1.0, 1.0,  1.0)));
	world.addObj(&Dir);
	Dir.writeUniform("Light");

	world.addObj( new ModelRenderer("assets/mesh/Test_Block.obj", "assets/textures/tex1.bmp"));
}
