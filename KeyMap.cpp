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
	KeyBindings[key].push_back(Func);
}

void KeyMap::onKeyPress(unsigned short key)
{
	std::vector<std::function<void(unsigned short)>> funcs = KeyBindings[key];

	for (std::function<void(unsigned short)> f : funcs)
		f(key);
}

KeyMap::~KeyMap()
{
}

void KeyMap::onKeyPress(char button, char action, char mods)
{
	onKeyPress(button | (mods << 8));
}
