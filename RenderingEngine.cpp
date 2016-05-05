#include "RenderingEngine.h"
#include "CoreEngine.h"
#include "Screen.h"
#include "Scene.h"
#include "Texture.h"
#include "UI.h"
#include "Light.h"
#include "RessourceHandler.h"


RenderingEngine::RenderingEngine(CoreEngine* parent, Screen* _screen) : ambient(string("assets/shaders/forward_ambient_vs.glsl"), string("assets/shaders/forward_ambient_fs.glsl"))
{
	this->_screen = _screen;
	_parent = parent;
	setup3DEngineState();
}

void RenderingEngine::load(RessourceHandler * loader)
{
	ambient.load(loader);
}

void RenderingEngine::init()
{
	ambient.build();
	UniformRegistry::registerShaderUniforms(&ambient);
	for (BaseLight* Light : Lights)
		UniformRegistry::registerShaderUniforms(Light->getShader());
}

void RenderingEngine::render(Scene * s)
{
	if (!_screen->isFocused())
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_BLEND);  //setting up Multipassing
	glBlendFunc(GL_ONE, GL_ONE);
	s->render(&ambient, RenderState::AMBIENT_PASS);
	
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	for (BaseLight* light : Lights)
		light->render();

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND); //resetting to standard State
	
	_screen->updateScreen();
}

void RenderingEngine::set3D()
{
	setup3DEngineState();
}

void RenderingEngine::registerGraphicObject(BaseLight * b)
{
	Lights.push_back(b);
}

RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::setup3DEngineState()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glFrontFace(GL_CCW); //defines the Front face having counterclockwise vertices for culling
	glCullFace(GL_BACK); //Deletes the Backface
	//glEnable(GL_CULL_FACE); //Enables Backfaceculling
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS); //Tells OpenGL that Framebuffer values may be overwritten if the new Fragment is closer
	glEnable(GL_DEPTH_TEST); //Enables Depth Test for Fragments
	glEnable(GL_ALPHA_TEST);
}