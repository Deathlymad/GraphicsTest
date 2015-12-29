#include "RenderingEngine.h"
#include "CoreEngine.h"



RenderingEngine::RenderingEngine(CoreEngine* parent, Screen* screen) : ambient("forward_ambient_vs.glsl", "forward_ambient_fs.glsl")
{
	this->screen = screen;
	_parent = parent;
	MainView = nullptr;
	setupInitialEngineState();
}

void RenderingEngine::render(Scene * s)
{
	if (!screen->isFocused())
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	s->render(&ambient, true);
	
	glEnable(GL_BLEND);  //setting up Multipassing
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	for (BaseLight* light : Lights)
		light->render();
	
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND); //resetting to standard State
	
	screen->updateScreen();
}

void RenderingEngine::registerGraphicObject(BaseLight * b)
{
	Lights.push_back(b);
	if (MainView)
		MainView->registerUniform(b->getShader(), true);
}

void RenderingEngine::registerGraphicObject(Camera * c) //needs to be changed
{
	MainView = c;
	initOnShaders([this](Shader* s, bool light) { if(MainView) MainView->registerUniform(s, light); });
}

RenderingEngine::~RenderingEngine()
{
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
	glEnable(GL_ALPHA_TEST);
}
