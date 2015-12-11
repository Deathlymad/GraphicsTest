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
	Dir = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 1000.0f, glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)));
	
	getEngine().registerGraphicObject(&Dir);
	Dir.writeUniform("Light");

	world.addObj( new ModelRenderer("assets/mesh/Test_Block.obj", "assets/textures/tex1.bmp"));
	world.addObj(&Dir);
}
