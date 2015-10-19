#define PRAGMALIB
#ifndef GLEW
	#define GLEW_STATIC
	#include <GL/glew.h>
	#ifdef PRAGMALIB
		#pragma comment(lib, "glew32s.lib")
		#pragma comment (lib, "OpenGL32.lib")
		#pragma comment(lib, "GLU32.lib")
	#endif
	#define GLEW
#endif
#ifndef GLFW
	#define GLFW_INCLUDE_NONE
	#include <GLFW/glfw3.h>
	#ifdef PRAGMALIB
		#pragma comment (lib, "glfw3.lib")
	#endif
	#define GLFW
#endif

#include <iostream>
#include <vector>

#include "Screen.h"

bool Screen::initializedGLFW = false;

void Screen::initGraphicContexCreation()
{
	if (initializedGLFW)
		return;
	//Context Setup with GLFW
	if (!glfwInit())
		std::cout << "GLFW" << "startup of GLFW errored" << std::endl;
	std::cout << "GLFW" << "started GLFW" << std::endl;
	
	initializedGLFW = true;
}

GLFWmonitor* Screen::getMonitor(unsigned short monitorID)
{
	int monitorCount;
	glfwGetMonitors(&monitorCount);
	std::vector<GLFWmonitor*> monitors = std::vector<GLFWmonitor*>();
	GLFWmonitor** temp = glfwGetMonitors(&monitorCount);
	monitors.insert(monitors.begin(), temp, temp + monitorCount);

	if (monitorID < monitors.size() && monitorID > 0)
		return monitors.at(monitorID);
	else if (monitorID == 0)
		return glfwGetPrimaryMonitor(); //0
	else
		return nullptr;
}


void Screen::createWindow(int width, int height, std::string title, char flags)
{
	if (flags == 0)
		flags = char(152);

	char tempVal = ((flags & 192) >> 6) + 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, tempVal);
	tempVal = (flags & 56) >> 3;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, tempVal);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE + ((flags & 4) >> 2) );
	winHandle = glfwCreateWindow(width, height, title.c_str(), getMonitor(flags & 3), NULL);
	

	if (!winHandle)
		return;

	std::cout << "Created Window" << std::endl;

	glfwSetInputMode(winHandle, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(winHandle);
}

void Screen::setupGraphicFunctions()
{
	if (initializedGLEW)
		return;

	std::cout << "GLEW" << "start init of GLEW" << std::endl;
	glewExperimental = true; // Needed for core profile
	std::cout << "GLEW" << "using experimental version of GLEW" << std::endl;
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW" << "Failed to initialize GLEW" << std::endl;
	}
	std::cout << "GLEW" << "done with GLEW" << std::endl;

	glGetError(); //catches the Invalid Enum Error that GLEW Throws, not a error but a bug
	std::cout << std::string("Running Versions: \n").c_str() <<
		"  Graphics Data:\n" << std::endl <<
		"    Graphic Renderer: " << (char*)glGetString(GL_RENDERER) << std::endl <<
		"    Graphics Vendor:  " << (char*)glGetString(GL_VENDOR) << std::endl <<
		"    OpenGL:           " << (char*)glGetString(GL_VERSION) << std::endl <<
		"    GLSL:             " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl <<
		"    GLFW:             " << (char*)glfwGetVersionString() << std::endl <<
		"    GLEW:             " << (char*)glewGetString(GLEW_VERSION) << std::endl;

	initializedGLEW = true;
}

Screen::Screen(std::string title)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &Width, &Height);
	createWindow(Width, Height, title, 0);

	setupGraphicFunctions();
}

Screen::Screen(int width, int height, std::string title)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	createWindow(width, height, title, 0);
	Width = width;
	Height = height;

	setupGraphicFunctions();
}

Screen::Screen(std::string title, char flags)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &Width, &Height);
	createWindow(Width, Height, title, flags);

	setupGraphicFunctions();
}

Screen::Screen(int width, int height, std::string title, char flags)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	createWindow(width, height, title, flags);
	Width = width;
	Height = height;

	setupGraphicFunctions();
}

void Screen::updateScreen()
{
	glfwSwapBuffers(winHandle);
	glfwPollEvents();
}

Screen::~Screen()
{
	glfwDestroyWindow(winHandle);
}
