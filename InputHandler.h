#include <set>
#include <vector>

struct GLFWwindow;
class Screen;

#pragma once
class InputHandler //abstract
{
public:
	enum InfoHandle
	{
		KeyPress,
		MouseMove,
		MouseClick
	};
	InputHandler(Screen* s, InfoHandle);
	InputHandler(InfoHandle);

	static void registerCallbacks(GLFWwindow*);

	~InputHandler();
protected:
	virtual void onKeyPress(char button, char action, char mods) {} //abstract, need to be overwritten
	virtual void onMouseMove(double x, double y) {} //abstract, need to be overwritten
	virtual void onMouseButton(char button, char action, char mods) {} //abstract, need to be overwritten

private:
	static std::vector<InputHandler*> Handles; //atores Update Links to all Input Handlers, common Ressource needs protection
	size_t pos;

	double x, y;

	std::vector<InfoHandle> handles;

	static bool registered;
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

