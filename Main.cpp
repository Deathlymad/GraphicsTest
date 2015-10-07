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


GLFWwindow* Window;

void initGLFW(int width, int heigth, std::string name)
{
	//Context Setup with GLFW
	if (!glfwInit())
		std::cout << "GLFW " << "startup of GLFW errored" << std::endl;
	std::cout << "GLFW " << "started GLFW" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Window = glfwCreateWindow(800, 600, "", NULL, NULL);

	std::cout << "GLFW " << "attempt to create window" << std::endl;
	if (!Window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwMakeContextCurrent(Window);
	glfwSetWindowShouldClose(Window, false);
	std::cout << "GLFW " << "Init of GLFW done" << std::endl;
}
void initGLEW()
{
	std::cout << "GLEW " << "start init of GLEW" <<std::endl;
	glewExperimental = true; // Needed for core profile
	std::cout << "GLEW " << "using experimental version of GLEW" <<std::endl;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW " << "Failed to initialize GLEW" <<std::endl;
	}
	std::cout << "GLEW " << "done with GLEW" << std::endl;

	glGetError(); //catches the Invalid Enum Error that GLEW Throws, not a error but a bug
}

int main()
{
	Screen s = Screen(800, 600, "Test", 154);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glFrontFace(GL_CCW); //defines the Front face having counterclockwise vertices for culling
	glCullFace(GL_BACK); //Deletes the Backface
	glEnable(GL_CULL_FACE); //Enables Backfaceculling */
	glEnable(GL_DEPTH_CLAMP);
	glDepthFunc(GL_LESS); //Tells OpenGL that Framebuffer values may be overwritten if the new Fragment is closer
	glEnable(GL_DEPTH_TEST); //Enables Depth Test for Fragments
	glAlphaFunc(GL_LESS, 1);
	glEnable(GL_ALPHA_TEST);

	Shader ambient = Shader("forward_ambient_vs.glsl", "forward_ambient_fs.glsl");

	std::vector<glm::vec3> vecs = std::vector<glm::vec3>();
	vecs.push_back( glm::vec3(-1.0f, -1.0f, 0.0f));
	vecs.push_back( glm::vec3(1.0f, -1.0f, 0.0f));
	vecs.push_back( glm::vec3(0.0f, 1.0f, 0.0f));

	std::vector<unsigned int> ind = std::vector<unsigned int>();
	ind.push_back(0);
	ind.push_back(1);
	ind.push_back(2);

	Mesh m = Mesh(vecs, ind);
	
	using namespace std::chrono;

	unsigned short frames = 0; //fps
	system_clock::time_point lasttick = std::chrono::system_clock::now();
	system_clock::time_point sleeper = std::chrono::system_clock::now();
	while (true)
	{
		sleeper = std::chrono::system_clock::now();

		//render frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ambient.bind();
		m.Draw();
		s.updateScreen();

		frames++;
		
		std::this_thread::sleep_for(milliseconds((int)std::floorf(16.666f)) - duration_cast<std::chrono::milliseconds>(system_clock::now() - sleeper));


		if (duration_cast<milliseconds>(system_clock::now() - lasttick) >= milliseconds(1000))
		{
			std::cout << "FPS" << std::to_string(frames) << std::endl;
			frames = 0;
			lasttick = system_clock::now();
		}
	}
}