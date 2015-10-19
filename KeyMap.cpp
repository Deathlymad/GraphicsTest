#define PRAGMALIB
#ifndef GLFW
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef PRAGMALIB
#pragma comment (lib, "glfw3.lib")
#endif
#define GLFW
#endif


#include "KeyMap.h"

KeyMap::KeyMap(Screen* s) : InputHandler(s), KeyBindings()
{}

void KeyMap::addKeyBind( unsigned short key, std::function<void(unsigned short)> Func)
{
	KeyBindings[key] = Func;
}

void KeyMap::onKeyPress(unsigned short key)
{
	if (KeyBindings[key] != nullptr)
	{ 
		auto temp = KeyBindings[key];
		temp(key);
	}
}

KeyMap::~KeyMap()
{
}

void KeyMap::onKeyPress(char button, char action, char mods)
{
	onKeyPress(button | (mods << 8));
}
