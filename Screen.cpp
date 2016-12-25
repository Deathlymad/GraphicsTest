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

#include <vector>

#include "Screen.h"

#include "Log.h"

bool Screen::initializedGLFW = false;

void Screen::initGraphicContexCreation()
{
	if (initializedGLFW)
		return;
	//Context Setup with GLFW
	if (!glfwInit())
		LOG << "GLFW" << "startup of GLFW errored" << "\n";
	LOG << "GLFW" << "started GLFW" << "\n";
	
	initializedGLFW = true;
}

GLFWmonitor* Screen::getMonitor(unsigned short monitorID)
{
	int monitorCount;
	glfwGetMonitors(&monitorCount);
	vector<GLFWmonitor*> monitors = vector<GLFWmonitor*>();
	GLFWmonitor** temp = glfwGetMonitors(&monitorCount);
	monitors.insert(monitors.begin(), temp, temp + monitorCount);

	if (monitorID < monitors.size() && monitorID > 0)
		return monitors.at(monitorID);
	else if (monitorID == 0)
		return glfwGetPrimaryMonitor(); //0
	else
		return nullptr; //actually uses the default screen in Fullscreen mode
}

void Screen::createWindow(int width, int height, string title, char flags)
{
	if (flags == 0)
		flags = char(152);

	char tempVal = ((flags & 192) >> 6) + 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, tempVal);
	tempVal = (flags & 56) >> 3;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, tempVal);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE + ((flags & 4) >> 2) );
	_winHandle = glfwCreateWindow(width, height, title.c_str(), getMonitor(flags & 3), NULL);
	

	if (!_winHandle)
		return;

	LOG << "Created Window" << "\n";
	//configure Window
	glfwSetInputMode(_winHandle, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(_winHandle, this);
	//register Callbacks
	glfwSetWindowSizeCallback(_winHandle, windowResize);
	glfwSetKeyCallback(_winHandle, keyCallback);
	glfwSetCursorPosCallback(_winHandle, cursorPosCallback);
	glfwSetMouseButtonCallback(_winHandle, mouseButtonCallback);
	glfwSetScrollCallback(_winHandle, mouseScrollCallback);
	
	//set window as current
	glfwMakeContextCurrent(_winHandle);
}

void Screen::setupGraphicFunctions()
{
	if (initializedGLEW)
		return;

	LOG << "start init of GLEW" << "\n";
	glewExperimental = true; // Needed for core profile
	LOG << "using experimental version of GLEW" << "\n";
	if (glewInit() != GLEW_OK) {
		LOG << "Failed to initialize GLEW" << "\n";
	}
	LOG << "done with GLEW" << "\n";

	glGetError(); //catches the Invalid Enum Error that GLEW Throws, not a error but a bug
	LOG << string("Running Versions: \n") +
		"  Graphics Data:\n\n" +
		"    Graphic Renderer: " + (char*)glGetString(GL_RENDERER) + "\n" +
		"    Graphics Vendor:  " + (char*)glGetString(GL_VENDOR) + "\n" +
		"    OpenGL:           " + (char*)glGetString(GL_VERSION) + "\n" +
		"    GLSL:             " + (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) + "\n" +
		"    GLFW:             " + (char*)glfwGetVersionString() + "\n" +
		"    GLEW:             " + (char*)glewGetString(GLEW_VERSION) + "\n";

	initializedGLEW = true;
}

Screen::Screen(string& title) : _width(0), _height(0), initializedGLEW(false), oldMouseX(0), oldMouseY(0)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &_width, &_height);
	createWindow(_width, _height, title, 0);

	setupGraphicFunctions();
}

Screen::Screen(int width, int height, string& title) : _width(width), _height(height), initializedGLEW(false), oldMouseX(0), oldMouseY(0)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	createWindow(width, height, title, 0);
	_width = width;
	_height = height;

	setupGraphicFunctions();
}

Screen::Screen(string& title, char flags) : _width(0), _height(0), initializedGLEW(false), oldMouseX(0), oldMouseY(0)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &_width, &_height);
	createWindow(_width, _height, title, flags);

	setupGraphicFunctions();
}

Screen::Screen(int width, int height, string& title, char flags) : _width(width), _height(height), initializedGLEW(false), oldMouseX(0), oldMouseY(0)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	createWindow(width, height, title, flags);

	setupGraphicFunctions();
}

bool Screen::isScreenClosed()
{
	return glfwWindowShouldClose(_winHandle) > 0; //will only work if mouse is not hidden
}

void Screen::updateScreen()
{
	glfwSwapBuffers(_winHandle);
	glfwPollEvents();
}

void Screen::enableCursor()
{
	glfwSetInputMode(_winHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Screen::disableCursor()
{
	glfwSetInputMode(_winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Screen::makeCurrent()
{
	glfwMakeContextCurrent(_winHandle);
}

bool Screen::isFocused()
{
	glfwPollEvents();
	return glfwGetWindowAttrib(_winHandle, GLFW_FOCUSED) != 0;
}

//Event Handling

void Screen::subscribe(ResizeCallback &callback)
{
	_resizeSubscribers.push_back(callback);
}
void Screen::subscribe(KeyCallback &callback)
{
	_KeySubscribers.push_back(callback);
}
void Screen::subscribe(MouseMoveCallback &callback)
{
	_MouseMoveSubscribers.push_back(callback);
}
void Screen::subscribe(MouseButtonCallback &callback)
{
	_ClickSubscribers.push_back(callback);
}
void Screen::subscribe(ScrollCallback &callback)
{
	_ScrollSubscribers.push_back(callback);
}


void Screen::onResize(int width, int height)
{
	_width = width;
	_height = height;

	for (ResizeCallback callback : _resizeSubscribers)
		callback( width, height);
}

void Screen::onKeyChange(char button, char action, char mods)
{
	for (KeyCallback callback : _KeySubscribers)
		callback(button, action, mods);
}
void Screen::onMouseMove(double x, double y)
{
	double changeX = oldMouseX - x;
	double changeY = oldMouseY - y;
	oldMouseX = x;
	oldMouseY = y;

	for (MouseMoveCallback callback : _MouseMoveSubscribers)
		callback(changeX, changeY, x, y);
}
void Screen::onMouseClick(char button, char action, char mods)
{
	for (MouseButtonCallback callback : _ClickSubscribers)
		callback(button, action, mods);
}
void Screen::onMouseScroll(double x, double y)
{
	for (ScrollCallback callback : _ScrollSubscribers)
		callback( x, y);
}



Screen::~Screen()
{
	if (glfwGetCurrentContext() == _winHandle)
		glfwDestroyWindow(_winHandle);
}


//GLFW Callbacks

void Screen::windowResize(GLFWwindow* win, int width, int height)
{
	Screen* handler = (Screen*)glfwGetWindowUserPointer(win);
	handler->onResize(width, height);
}

void Screen::keyCallback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	Screen* handler = (Screen*)glfwGetWindowUserPointer(win);
	handler->onKeyChange(key, action, mods);
}
void Screen::cursorPosCallback(GLFWwindow* win, double xpos, double ypos)
{
	Screen* handler = (Screen*)glfwGetWindowUserPointer(win);
	handler->onMouseMove(xpos, ypos);
}
void Screen::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods)
{
	Screen* handler = (Screen*)glfwGetWindowUserPointer(win);
	handler->onMouseClick(button, action, mods);
}
void Screen::mouseScrollCallback(GLFWwindow* win, double xOffset, double yOffset)
{
	Screen* handler = (Screen*)glfwGetWindowUserPointer(win);
	handler->onMouseScroll(xOffset, yOffset);
}