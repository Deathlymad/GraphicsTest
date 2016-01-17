#include "TestGame.h"
#include "Def.h"
#include "UIPart.h"

NSP_GLM


TestGame::TestGame() : Game(),
Light(vec3(1.0f, 0.3f, 0.2f), 1.0f, vec3(0.0f, 0.0f, 1.0f)),
Light1(vec3(0.0f, 0.9f, 0.0f), 1.0f, Attenuation(1, 2, 4), vec3(0.0f, 1.2f, 0.0f)),
Light2(vec3(0.0f, 0.0f, 0.9f), 3.0f, Attenuation(1, 2, 4), vec3(0.0f, 1.2f, 0.0f), vec3( 0.0f, -1.0f, 0.0f), 0.8f),
m( 0, 1, 2),
obj("assets/mesh/Test_Block.obj", "assets/textures/Test_tex2.bmp"),
trans( 0, vec3(), vec3(), vec3(1.0f, 1.0f, 1.0f))
{
	//Rendering Registry
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light1);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light2);
	m.init(Light.getShader());
	m.init(Light1.getShader());
	m.init(Light2.getShader());

	getEngine()->getGraphicEngine()->initOnShaders([this](Shader* s, bool) {trans.init(s); });

	//scene Registry
	trans.add(&obj);

	m.add(&trans);

	Light.add(&m);
	Light1.add(&m);
	Light2.add(&m);

	world.addObj(&m);
	
	Menu = new UI(this);
	Menu->add(new UIButton(Menu, vec2(-0.2, -0.2), vec2(0.2, 0.2), -1, [this] {toggleMenu(); }));
}

TestGame::~TestGame()
{
}