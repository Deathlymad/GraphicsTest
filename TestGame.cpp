#include "TestGame.h"
#include "Def.h"
#include "UIPart.h"

NSP_GLM


TestGame::TestGame() : Game(),
	Light(vec3(1.0f, 0.3f, 0.2f), 1.0f, vec3(0.0f, 0.0f, 1.0f)),
	Light1(vec3(0.0f, 0.9f, 0.0f), 1.0f, 2, 1, 1, vec3(0.0f, 1.2f, 0.0f)),
	Light2(vec3(0.0f, 0.0f, 0.9f), 1.0f, 1, 2, 4, vec3(0.0f, 1.2f, 0.0f), vec3(0.0f, -1.0f, 0.0f), 0.3f),
	m(0, 2, 32),
	obj("assets/mesh/untitled.obj", "assets/textures/Test_tex2.bmp"),
	trans(0, vec3(), vec3(), vec3(1.0f, 1.0f, 1.0f)),
	_testField( getEngine()->getThreadManager(), &getScene().getCam(), 50, 50)
{
	//Rendering Registry
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light1);
	getEngine()->getGraphicEngine()->registerGraphicObject(&Light2);

	//scene Registry
	//trans.add(&obj);
	trans.add(&_testField);

	getScene().addObj(&trans);

	/*
	m.add(&trans);

	Light.add(&m);
	Light1.add(&m);
	Light2.add(&m);

	getScene().addObj(&Light);
	getScene().addObj(&Light1);
	getScene().addObj(&Light2);

	getScene().setRootUI(UI(this));
	getScene().addObj(new UIButton(getScene().getMenu(), vec2(-0.2f, -0.2f), vec2(0.2f, 0.2f), -1, [this] {getScene().toggleMenu(); }));
	getScene().addObj(new UIPart(getScene().getMenu(), vec2(1.0f, -1.0f), vec2(1.0f, 1.0f))); //add Background
	*/
}

TestGame::~TestGame()
{
}