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
vector<Screen*> Screen::_winPtr;

void Screen::initGraphicContexCreation()
{
	if (initializedGLFW)
		return;
	//Context Setup with GLFW
	if (!glfwInit())
		cout << "GLFW" << "startup of GLFW errored" << endl;
	cout << "GLFW" << "started GLFW" << endl;
	
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

unsigned int Screen::getPtr(GLFWwindow * key, unsigned int min, unsigned int max)
{
	if (min > max) {
		return min;
	}
	else if (_winPtr.size() == 0)
		return 0;
	else {
		unsigned int mid = (min + max) / 2;
		if (mid >= _winPtr.size())
			return _winPtr.size();
		int comp = int(_winPtr[mid]->_winHandle) - int(key);
		if (comp == 0) {
			return mid;
		}
		else if (comp < 0) {
			return getPtr(key, mid + 1, max);
		}
		else {
			return getPtr(key, min, mid - 1);
		}
	}
}

void Screen::onWindowResize(GLFWwindow * win, int width, int height)
{
	Screen* s = _winPtr[getPtr(win, 0, _winPtr.size() - 1)];
	s->_width = width;
	s->_height = height;
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

	cout << "Created Window" << endl;
	
	glfwSetInputMode(_winHandle, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(_winHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(_winHandle, onWindowResize);
	_winPtr.insert(_winPtr.begin() + getPtr(_winHandle, 0, _winPtr.size() - 1), this);

	glfwMakeContextCurrent(_winHandle);
}

void Screen::setupGraphicFunctions()
{
	if (initializedGLEW)
		return;

	cout << "GLEW" << "start init of GLEW" << endl;
	glewExperimental = true; // Needed for core profile
	cout << "GLEW" << "using experimental version of GLEW" << endl;
	if (glewInit() != GLEW_OK) {
		cout << "GLEW" << "Failed to initialize GLEW" << endl;
	}
	cout << "GLEW" << "done with GLEW" << endl;

	glGetError(); //catches the Invalid Enum Error that GLEW Throws, not a error but a bug
	cout << string("Running Versions: \n").c_str() <<
		"  Graphics Data:\n" << endl <<
		"    Graphic Renderer: " << (char*)glGetString(GL_RENDERER) << endl <<
		"    Graphics Vendor:  " << (char*)glGetString(GL_VENDOR) << endl <<
		"    OpenGL:           " << (char*)glGetString(GL_VERSION) << endl <<
		"    GLSL:             " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << endl <<
		"    GLFW:             " << (char*)glfwGetVersionString() << endl <<
		"    GLEW:             " << (char*)glewGetString(GLEW_VERSION) << endl;

	initializedGLEW = true;
}

Screen::Screen(string title)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &_width, &_height);
	createWindow(_width, _height, title, 0);

	setupGraphicFunctions();
}

Screen::Screen(int width, int height, string title)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	createWindow(width, height, title, 0);
	_width = width;
	_height = height;

	setupGraphicFunctions();
}

Screen::Screen(string title, char flags)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &_width, &_height);
	createWindow(_width, _height, title, flags);

	setupGraphicFunctions();
}

Screen::Screen(int width, int height, string title, char flags)
{
	if (!initializedGLFW)
		initGraphicContexCreation();

	createWindow(width, height, title, flags);
	_width = width;
	_height = height;

	setupGraphicFunctions();
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

Screen::~Screen()
{
	if (!_winPtr.empty())
		_winPtr.erase(_winPtr.begin() + getPtr(_winHandle, 0, _winPtr.size() - 1));
	glfwDestroyWindow(_winHandle);
}
