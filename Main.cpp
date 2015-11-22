#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <glm/common.hpp>

#define PRAGMALIB
#ifndef GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#ifdef PRAGMALIB
#pragma comment(lib, "glew32s.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment(lib, "GLU32.lib")
#endif
#endif
#ifndef GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef PRAGMALIB
#pragma comment (lib, "glfw3.lib")
#endif
#define GLFW
#endif


#include "Shader.h"
#include "Mesh.h"
#include "Screen.h"
#include "KeyMap.h"
#include "Clock.h"
#include "Camera.h"
#include "Scene.h"
#include "ModelRenderer.h"
#include "Light.h"

#include "Game.h"

Screen* s;
Shader* ambient, *directional;
Mesh* m;
DirectionalLight Dir;
Scene* scene;
Clock* MainLoop, *UpdateLoop;
KeyMap*  map;

void initGraphics()
{
	s = new Screen(1366, 768, "Test", char(154));

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glFrontFace(GL_CCW); //defines the Front face having counterclockwise vertices for culling
	glCullFace(GL_BACK); //Deletes the Backface
	glEnable(GL_CULL_FACE); //Enables Backfaceculling */
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS); //Tells OpenGL that Framebuffer values may be overwritten if the new Fragment is closer
	glEnable(GL_DEPTH_TEST); //Enables Depth Test for Fragments
	glAlphaFunc(GL_LESS, 1);
	glEnable(GL_ALPHA_TEST);

	ambient = new Shader("forward_ambient_vs.glsl", "forward_ambient_fs.glsl");
	Dir = DirectionalLight( glm::vec3(1.0f, 1.0f, 1.0f), 1000.0f, glm::normalize(glm::vec3( 1.0f, 1.0f, 1.0f)));
	Dir.writeUniform("Light");

	scene = new Scene();
	scene->addObj( new ModelRenderer("assets/mesh/stein_einfach.obj",""));

}

void multipassRender()
{
	//UpdateLoop->pause();
	scene->render(ambient);
	/*
	//needs to pause Update for the passes since depth would change
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	scene->render(Dir.getShader());
	
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	UpdateLoop->resume();*/
	s->updateScreen();
}

void initKeys()
{
	map = new KeyMap(s);
	map->addKeyBind(0, [ ](unsigned short) {MainLoop->shutdown(); UpdateLoop->shutdown(); }, "Shutdown"); //set Escape Key
	scene->init(ambient, map);
	map->launchKeyMap();
	scene->init(Dir.getShader());
}

int main()
{
	Game Test;

	Test.Start();

	Test.~Game();

	/*
	UpdateLoop = new Clock(initKeys, [] { scene->update(); }, 60); //needs to be as fast as the thread

	MainLoop = new Clock(initGraphics, multipassRender, 60);

	while (MainLoop->isRunning()) {}*/
}