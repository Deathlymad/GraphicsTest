#include <functional>
#include <string>
#include <vector>
#include "Def.h"

NSP_STD

struct GLFWwindow;
struct GLFWmonitor;


#pragma once
class Screen
{
public:

	static void initGraphicContexCreation();
	static GLFWmonitor* getMonitor(unsigned short monitorID);


	Screen(string title);
	Screen(int width, int height, string title);
	Screen(string title, char flags);
	Screen(int width, int height, string title, char flags);

	bool isScreenClosed();

	void updateScreen();

	void enableCursor();
	void disableCursor();

	void handleWindow(function<void(GLFWwindow*)> f)
	{
		f(_winHandle);
	}

	void makeCurrent();
	bool isFocused();

	int getWidth() { return _width; }
	int getHeight() { return _height; }

	~Screen();
private:
	int _width, _height;

	GLFWwindow* _winHandle;
	static unsigned int getPtr(GLFWwindow*, unsigned int, unsigned int);
	static vector<Screen*> _winPtr;

	static void onWindowResize(GLFWwindow*, int, int);
	void createWindow(int width, int height, string title, char flags);
	void setupGraphicFunctions();

	static bool initializedGLFW;
	bool initializedGLEW;
};

