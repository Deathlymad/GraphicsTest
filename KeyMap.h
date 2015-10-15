#include <map>
#include <vector>
#include <functional>

#include "InputHandler.h"

struct GLFWwindow;

#pragma once
class KeyMap : InputHandler
{
public:
	KeyMap(Screen* s);

	void addKeyBind( unsigned short key, std::function<void(unsigned short)> Func);

	void onKeyPress( unsigned short);

	~KeyMap();

private:
	std::map<unsigned short, std::vector<std::function<void(unsigned short)>>> KeyBindings; // short in binary 00000 3 bit modifier Key (Alt, Shift, Ctrl) 8 bit key
};