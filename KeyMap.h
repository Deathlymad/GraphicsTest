#include <map>
#include <vector>
#include <functional>

#include "InputHandler.h"

#pragma once
class KeyMap : InputHandler
{
public:
	KeyMap(Screen* s);

	void addKeyBind( unsigned short key, std::function<void(unsigned short)> Func);

	void onKeyPress( unsigned short);

	~KeyMap();
protected:
	void onKeyPress(char button, char action, char mods);
private:
	std::map<unsigned short, std::vector<std::function<void(unsigned short)>>> KeyBindings; // short in binary 00000 3 bit modifier Key (Alt, Shift, Ctrl) 8 bit key
};