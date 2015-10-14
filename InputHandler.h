#include <set>
#include <vector>

struct GLFWwindow;
class Screen;

#pragma once
class InputHandler //abstract , To Do Gamepad/Joystick
{
	enum InputType
	{
		KEYBOARD,
		MOUSE,
		JOYSTICK,
		GAMEPAD,
		OTHER
	};
public:
	InputHandler(Screen* s);

	~InputHandler();
protected:
	virtual void onKeyPress(char button, char action, char mods) {} //abstract, need to be overwritten
	virtual void onMouseMove(double x, double y) {} //abstract, need to be overwritten
	virtual void onMouseButton(char button, char action, char mods) {} //abstract, need to be overwritten

private:
	std::set<InputType> EventCalls; //stores all the types of events that are accepted

	static std::vector<InputHandler*> Handles; //atores Update Links to all Input Handlers

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

