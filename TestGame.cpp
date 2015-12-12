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
	spot = SpotLight(glm::vec3(0.4, 0.8, 0.2), 1.0f, Attenuation(1.0f, 1.0f, 1.0f), glm::vec3(-2.0f, -2.0f, -2.0f), glm::normalize(glm::vec3(-1.0, 1.0, -1.0)), -10.0f);
	world.addObj(&spot);
	spot.writeUniform("sLight");
	world.addObj( new ModelRenderer("assets/mesh/Test_Block.obj", "assets/textures/tex1.bmp"));
}
