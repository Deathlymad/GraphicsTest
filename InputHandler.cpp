#include "InputHandler.h"

std::vector<InputHandler*> InputHandler::Handles;

InputHandler::InputHandler(Screen* s)
{
}


InputHandler::~InputHandler()
{
}

/*GLFW Callback Functions*/

void InputHandler::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
}

void InputHandler::cursorPosCallback(GLFWwindow * window, double xpos, double ypos)
{
}

void InputHandler::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
}
