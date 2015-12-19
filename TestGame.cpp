#include "TestGame.h"



TestGame::TestGame() : Game()
{
	
	pt = PointLight( glm::vec3(1.0f, 0.0f, 0.0f), 10.0f, Attenuation(6.0f, 0.0f, 0.0f), glm::normalize(glm::vec3( 1.0f, 1.0f, 1.0f)));
	pt.createUniforms("pLight");
	Dir = DirectionalLight( glm::vec3(0.1f, 0.6f, 0.8f), 1.0f, glm::vec3( 0.0f, 0.0f, 1.0f));
	Dir.createUniforms("Light");
	
	m = Material(32, 2);
	obj = ModelRenderer("assets/mesh/untitled.obj", "assets/textures/tex1.bmp");

	//Rendering Registry
	obj.init( getEngine()->getGraphicEngine());
	getEngine()->getGraphicEngine()->registerGraphicObject(&pt);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Dir);

	//scene Registry
	world.addObj(&pt);
	world.addObj(&Dir);
	world.addObj(&m);
	world.addObj(&obj);
}

TestGame::~TestGame()
{
}
