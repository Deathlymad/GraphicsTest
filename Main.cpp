#include <chrono>
#include <iostream>
#include <thread>
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

Screen* s;
Shader* ambient;
Mesh* m;
Camera* c;
Scene* scene;

void initGraphics()
{
	s = new Screen(1366, 768, "Test", char(154));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glFrontFace(GL_CCW); //defines the Front face having counterclockwise vertices for culling
	glCullFace(GL_BACK); //Deletes the Backface
	glEnable(GL_CULL_FACE); //Enables Backfaceculling */
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS); //Tells OpenGL that Framebuffer values may be overwritten if the new Fragment is closer
	glEnable(GL_DEPTH_TEST); //Enables Depth Test for Fragments
	glAlphaFunc(GL_LESS, 1);
	glEnable(GL_ALPHA_TEST);

	ambient = new Shader("forward_ambient_vs.glsl", "forward_ambient_fs.glsl");

	scene = new Scene();
	scene->addObj(new ModelRenderer("assets/mesh/stein_einfach.obj",""));
}

int main()
{

	Clock MainLoop(initGraphics, [] { scene->update(); scene->render(ambient); s->updateScreen(); }, 60);

	MainLoop.run();

	while (!scene) {} //waiting for object handles to construct

	KeyMap k = KeyMap(s);
	k.addKeyBind(0, [&MainLoop](unsigned short) {MainLoop.shutdown(); }, "Shutdown"); //set Escape Key
	scene->init(ambient, &k);
	k.launchKeyMap();

	while (MainLoop.isRunning())
		std::cout << "FPS: " << MainLoop.getLastTPS() << std::endl;
}