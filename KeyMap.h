#include <vector>
#include <string>
#include <thread>
#include <functional>

#include "Clock.h"
#include "InputHandler.h"

#pragma once
class KeyMap : InputHandler
{
	struct KeyBind
	{
		unsigned short key;
		std::function<void(unsigned short)> callback;
		std::string name;
		bool isPressed;
	};
public:
	KeyMap(Screen* s);
	KeyMap(KeyMap&);

	void launchKeyMap() { KeyTick.run(); }

	void addKeyBind( unsigned short key, std::function<void(unsigned short)> Func, std::string name);

	void onKeyPress( unsigned short);

	~KeyMap();
protected:
	void onKeyPress(char button, char action, char mods);
private:
	std::vector<KeyBind> KeyBindings; // short in binary 00000 3 bit modifier Key (Alt, Shift, Ctrl) 8 bit key

	static size_t find(unsigned short, std::vector<KeyBind>*, int, int);

	static void updateKeyMap(KeyMap*);

	Clock KeyTick;
};