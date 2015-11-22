#include "RenderingEngine.h"



RenderingEngine::RenderingEngine(Screen* screen) : ThreadExclusiveObject<RenderingEngine>(*this), ambient("forward_ambient_vs.glsl", "forward_ambient_fs.glsl")
{
	this->screen = screen;
	setupInitialEngineState();
}

void RenderingEngine::render(Scene * s)
{
	RenderingEngine* ptr = (RenderingEngine*)malloc(sizeof(RenderingEngine));
	claim(*ptr);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	s->render(&ambient);

	for (BaseLight* light : Lights)
		multipassRender(s, light->getShader());

	screen->updateScreen();
	unclaim(*ptr);
}

void RenderingEngine::registerGraphicObject(BaseLight *)
{
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::setupInitialEngineState()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glFrontFace(GL_CCW); //defines the Front face having counterclockwise vertices for culling
	glCullFace(GL_BACK); //Deletes the Backface
	glEnable(GL_CULL_FACE); //Enables Backfaceculling */
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS); //Tells OpenGL that Framebuffer values may be overwritten if the new Fragment is closer
	glEnable(GL_DEPTH_TEST); //Enables Depth Test for Fragments
	glAlphaFunc(GL_LESS, 1);
	glEnable(GL_ALPHA_TEST);
}

void RenderingEngine::multipassRender(Scene* scene, Shader * shader)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	scene->render(shader);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}
