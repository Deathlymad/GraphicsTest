#include "TestGame.h"
#include "Def.h"
#include "UIPart.h"

NSP_GLM


TestGame::TestGame() : Game(),
Light(vec3(1.0f, 0.3f, 0.2f), 1.0f, vec3(0.0f, 0.0f, 1.0f)),
Light1(vec3(0.0f, 0.9f, 0.0f), 1.0f, Attenuation(1, 2, 4), vec3(0.0f, 1.2f, 0.0f)),
Light2(vec3(0.0f, 0.0f, 0.9f), 3.0f, Attenuation(1, 2, 4), vec3(0.0f, 1.2f, 0.0f), vec3( 0.0f, -1.0f, 0.0f), 0.8f),
m( 0, 2, 32),
obj("assets/mesh/Test_Block.obj", "assets/textures/Test_tex2.bmp"),
trans( 0, vec3(), vec3(), vec3(1.0f, 1.0f, 1.0f))
{
	//Rendering Registry
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light1);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light2);

	//scene Registry
	trans.add(&obj);

	m.add(&trans);

	Light.add(&m);
	Light1.add(&m);
	Light2.add(&m);

	_world.addObj(&m);
	
	UI* menu = new UI(this);
	menu->addEvent(0, [this, menu](unsigned short, KeyMap::KeyState)
	{
		if (menu->isActive())
			menu->deactivate();
	}, "Menu", KeyMap::KeyState::ONPRESS, 0);
	menu->add(new UIButton(menu, vec2(-0.2, -0.2), vec2(0.2, 0.2), -1, [this] {Terminate(); }));
	menu->add(new UIText(menu, vec2(-1.0, 0.5), vec2(-0.5, 1.0)));

	setMenu(menu);

	getEngine()->getGraphicEngine()->init();
}

TestGame::~TestGame()
{
}