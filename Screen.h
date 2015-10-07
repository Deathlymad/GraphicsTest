#include <functional>
#include <string>

struct GLFWwindow;


#pragma once
class Screen
{
public:

	static void initGraphicContexCreation();
	static GLFWmonitor* getMonitor(unsigned short monitorID);


	Screen( std::string title);
	Screen( int width, int height, std::string title);
	Screen(std::string title, char flags);
	Screen(int width, int height, std::string title, char flags);

	void updateScreen();

	~Screen();
private:
	int Width, Height;
	GLFWwindow* winHandle;
	void createWindow(int width, int height, std::string title, char flags);
	void setupGraphicFunctions();

	static bool initializedGLFW;
	bool initializedGLEW;
};

