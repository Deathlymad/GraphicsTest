#include <set>
#include <vector>

struct GLFWwindow;
class Screen;

#pragma once
class InputHandler //abstract
{
public:
	InputHandler(Screen* s);
	InputHandler();
	~InputHandler();

	static void registerCallbacks(GLFWwindow*);

protected:
	virtual void onKeyPress(char button, char action, char mods) {} //abstract, needs to be overwritten
	virtual void onMouseMove(double x, double y) {} //abstract, needs to be overwritten
	virtual void onMouseButton(char button, char action, char mods) {} //abstract, needs to be overwritten

private:
	static std::vector<InputHandler*> Handles; //atores Update Links to all Input Handlers, common Ressource needs protection
	size_t pos;

	double x, y;

	static bool registered;
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

