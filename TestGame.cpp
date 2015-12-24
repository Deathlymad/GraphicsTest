#include "TestGame.h"
#include "Def.h"

NSP_GLM


TestGame::TestGame() : Game(),
pt(vec3(1.0f, 0.0f, 0.0f), 10.0f, Attenuation(1.0f, 0.0f, 0.0f), vec3(2.0f, 2.0f, 2.0f)),
Dir(vec3(0.1f, 0.6f, 0.8f), 1.0f, vec3(0.0f, 0.0f, 1.0f)),
m( 0, 1, 8),
obj("assets/mesh/untitled.obj", "assets/textures/tex1.bmp"),
trans( 0, vec3(), vec3(0.75f, 0.75f, 0.75f), vec3(1.0f, 2.0f, 1.0f))
{
	pt.createUniforms("pLight");
	Dir.createUniforms("Light");

	//Rendering Registry
	getEngine()->getGraphicEngine()->registerGraphicObject(&pt);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Dir);

	getEngine()->getGraphicEngine()->initOnShaders([this](Shader*s) {m.init(s); });
	getEngine()->getGraphicEngine()->initOnShaders([this](Shader* s) {trans.init(s); });

	//scene Registry
	trans.add(&obj);
	m.add(&trans);

	Dir.add(&m);
	pt.add(&m);

	world.addObj(&pt);
	world.addObj(&Dir);

}

TestGame::~TestGame()
{
}
