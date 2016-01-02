#include <set>
#include <vector>
#include "Def.h"

NSP_STD

struct GLFWwindow;
class Screen;

#pragma once
class InputHandler //abstract
{
public:
	enum MouseState
	{
		ONPRESS = 1,
		ONRELEASE = 2,
		ONHOLD = 4,
		ONRIGHTMOUSE = 8,
		ONSCROLL = 16
	};

	InputHandler(Screen* s);
	InputHandler();
	~InputHandler();

	static void registerCallbacks(GLFWwindow*);

protected:
	virtual void onKeyPress(char button, char action, char mods) {} //abstract, needs to be overwritten
	virtual void onMouseMove(double x, double y) {} //abstract, needs to be overwritten
	virtual void onMouseButton(char button, char action, char mods) {} //abstract, needs to be overwritten
	virtual void onScroll(double x, double y) {}

	void getCursorPos(double &x, double &y) { x = _x; y = _y; }
private:
	static vector<InputHandler*> Handles; //atores Update Links to all Input Handlers, common Ressource needs protection
	size_t pos;

	double _x, _y;

	static bool registered;
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

