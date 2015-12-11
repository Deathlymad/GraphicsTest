#include "RenderingEngine.h"



RenderingEngine::RenderingEngine(Screen* screen) : ThreadExclusiveObject<RenderingEngine>(*this) , ambient("forward_ambient_vs.glsl", "forward_ambient_fs.glsl", false)
{
	this->screen = screen;
	setupInitialEngineState();
}

void RenderingEngine::render(Scene * s)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	s->render(&ambient); //generates depth buffer
	
	glEnable(GL_BLEND);  //setting up Multipassing
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_LEQUAL);

	for (BaseLight* light : Lights)
		s->render(light->getShader());
	
	glDepthFunc(GL_LESS);
	glDepthMask(true);
	glDisable(GL_BLEND); //resetting to standard State

	screen->updateScreen();
}

void RenderingEngine::registerGraphicObject(BaseLight * b)
{
	Lights.push_back(b);
	if (MainView)
		MainView->registerUniform(b->getShader());
}

void RenderingEngine::registerGraphicObject(Camera * c)
{
	MainView = c;
	initOnShaders([this](Shader* s) {MainView->registerUniform(s); });
}


RenderingEngine::~RenderingEngine()
{
	screen = nullptr;
}

void RenderingEngine::setupInitialEngineState()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glFrontFace(GL_CCW); //defines the Front face having counterclockwise vertices for culling
	glCullFace(GL_BACK); //Deletes the Backface
	glEnable(GL_CULL_FACE); //Enables Backfaceculling */
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS); //Tells OpenGL that Framebuffer values may be overwritten if the new Fragment is closer
	glEnable(GL_DEPTH_TEST); //Enables Depth Test for Fragments
}
