#include <functional>
#include <string>
#include <vector>

#include "Def.h"
#include "ScreenCallbacks.h"

NSP_STD

struct GLFWwindow;
struct GLFWmonitor;

#pragma once
class Screen
{
public:
	Screen(string& title);
	Screen(int width, int height, string& title);
	Screen(string& title, char flags);
	Screen(int width, int height, string& title, char flags);

	bool isScreenClosed();

	void updateScreen();

	void enableCursor();
	void disableCursor();

	void subscribe(ResizeCallback&);
	void subscribe(KeyCallback&);
	void subscribe(MouseMoveCallback&);
	void subscribe(MouseButtonCallback&);
	void subscribe(ScrollCallback&);

	void makeCurrent();
	bool isFocused();

	int getWidth() { return _width; }
	int getHeight() { return _height; }

	~Screen();
private:
	int _width, _height;
	GLFWwindow* _winHandle;

	static bool initializedGLFW;
	static void initGraphicContexCreation();
	static GLFWmonitor* getMonitor(unsigned short monitorID);
	bool initializedGLEW;
	void createWindow(int width, int height, string title, char flags);
	void setupGraphicFunctions();


	void onResize( int width, int height);
	vector<ResizeCallback> _resizeSubscribers;
	void onKeyChange(char button, char action, char mods);
	vector<KeyCallback> _KeySubscribers;
	void onMouseMove(double x, double y);
	double oldMouseX, oldMouseY;
	vector<MouseMoveCallback> _MouseMoveSubscribers;
	void onMouseClick(char button, char action, char mods);
	vector<MouseButtonCallback> _ClickSubscribers;
	void onMouseScroll(double x, double y);
	vector<ScrollCallback> _ScrollSubscribers;

	//GLFW callbacks
	static void windowResize(GLFWwindow*, int, int);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};