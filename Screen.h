#include <functional>
#include <string>
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

	void updateScreen();

	void handleWindow(function<void(GLFWwindow*)> f)
	{
		f(winHandle);
	}

	void makeCurrent();
	bool isFocused();

	int getWidth() { return Width; }
	int getHeight() { return Height; }

	~Screen();
private:
	int Width, Height;
	GLFWwindow* winHandle;
	void createWindow(int width, int height, string title, char flags);
	void setupGraphicFunctions();

	static bool initializedGLFW;
	bool initializedGLEW;
};

