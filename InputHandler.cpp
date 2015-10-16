#define PRAGMALIB
#ifndef GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef PRAGMALIB
#pragma comment (lib, "glfw3.lib")
#endif
#define GLFW
#endif

#include "Screen.h"
#include "InputHandler.h"

bool InputHandler::registered = false;
std::vector<InputHandler*> InputHandler::Handles;

InputHandler::InputHandler(Screen* s)
{
	if (!registered)
		s->handleWindow(registerCallbacks);
	Handles.push_back(this);

	x = 0;
	y = 0;
}

InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}

/*GLFW Callback Functions*/

void InputHandler::registerCallbacks(GLFWwindow* w)
{
	glfwSetKeyCallback(w, keyCallback);
	glfwSetCursorPosCallback(w, cursorPosCallback);
	glfwSetMouseButtonCallback(w, mouseButtonCallback);
}

void InputHandler::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	for (InputHandler* handle : Handles)
		handle->onKeyPress(key, action, mods);
}

void InputHandler::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
	for (InputHandler* handle : Handles)
	{
		handle->onMouseMove(xpos - handle->x, ypos - handle->y);
		handle->x = xpos;
		handle->y = ypos;
	}
}

void InputHandler::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	for (InputHandler* handle : Handles)
		handle->onMouseButton(button, action, mods);
}
