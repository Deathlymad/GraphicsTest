#include <vector>
#include <string>
#include <thread>
#include <functional>

#include "Clock.h"
#include "InputHandler.h"
#include "Def.h"

NSP_STD

#pragma once
class KeyMap : InputHandler
{
	struct KeyBind
	{
		unsigned short key;
		function<void(unsigned short)> callback;
		string name;
		bool isPressed;
	};
public:
	KeyMap(Screen* s);
	KeyMap(KeyMap&);
	KeyMap();

	void launchKeyMap() { KeyTick.run(); }

	void addKeyBind( unsigned short key, function<void(unsigned short)> Func, string name);

	void onKeyPress( unsigned short);

	KeyMap& operator = (KeyMap&);

	~KeyMap();
protected:
	void onKeyPress(char button, char action, char mods);
private:
	vector<KeyBind> KeyBindings; // short in binary 00000 3 bit modifier Key (Alt, Shift, Ctrl) 8 bit key

	static size_t find(unsigned short, vector<KeyBind>*, int, int);

	static void updateKeyMap(KeyMap*);

	Clock KeyTick;
};