#include "TestGame.h"
#include "ModelRenderer.h"



TestGame::TestGame() : Game()
{
	pt = PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, Attenuation(6.0f, 0.0f, 0.0f), glm::vec3( 1.0f, 1.0f, 1.0f));
	pt.writeUniform("pLight");

	Dir = DirectionalLight(glm::vec3(0.1f, 0.0f, 0.3f), 10.0f, glm::normalize(glm::vec3( 1.0f, 1.0f,  1.0f)));
	Dir.writeUniform("Light");

	world.addObj(&pt);
	world.addObj(&Dir);
	world.addObj( new ModelRenderer("assets/mesh/Test_Block.obj", "assets/textures/tex1.bmp"));
}

TestGame::~TestGame()
{
}
