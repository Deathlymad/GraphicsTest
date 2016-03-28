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

KeyMap::KeyMap(Screen* s) : InputHandler(s), _activated(true)
{
}

KeyMap::KeyMap(KeyMap & k): InputHandler(k), KeyBindings(k.KeyBindings), _activated(true)
{
}

KeyMap::KeyMap() : InputHandler(), _activated(true)
{
}

void KeyMap::addKeyBind( unsigned short key, function<void(unsigned short, KeyState)> Func, string name, int trig)
{
	KeyBind k;
	k.key = key;
	k.callback = Func;
	k.name = name;
	k.trigger = trig;
	k.isPressed = false;
	int newPos = find(key, &KeyBindings, 0, KeyBindings.size());
	KeyBindings.insert(KeyBindings.begin() + newPos, k);
}

void KeyMap::onKeyPress(unsigned short key)
{
	KeyBind temp = KeyBindings[find(key, &KeyBindings, 0, KeyBindings.size())];

	if (temp.trigger & ONHOLD)
		temp.callback(key, ONHOLD);
}

KeyMap & KeyMap::operator=(KeyMap & other)
{
	KeyBindings = other.KeyBindings;

	_activated = other._activated;
	return *this;
}


KeyMap::~KeyMap()
{
}

void KeyMap::onKeyPress(char button, char action, char mods)
{
	unsigned short key = (button & 255) | ((mods & 7) << 8);
	size_t pos= find(key, &KeyBindings, 0, KeyBindings.size());
	if (pos >= KeyBindings.size())
		return;
	if (key != KeyBindings[pos].key)
		return;

	int state = (action ? action : 4);

	if ((KeyBindings[pos].trigger & state) && _activated)
	{
		KeyBindings[pos].callback(key, KeyState(state));
	}
}

size_t KeyMap::find(unsigned short key, vector<KeyBind>* arr, int min, int max)
{
	if (min > max) {
		return min;
	}
	else if (arr->size() == 0)
		return 0;
	else {
		unsigned int mid = (min + max) / 2;
			if (mid >= arr->size())
				return arr->size();
		int comp = (*arr)[mid].key - key;
		if (comp == 0) {
			return mid;
		}
		else if (comp < 0) {
			return find(key, arr, mid + 1, max);
		}
		else {
			return find(key, arr, min, mid - 1);
		}
	}
}

void KeyMap::updateKeyMap(KeyMap * k)
{
	if (!k->_activated)
		return;

	for (unsigned int i = k->KeyBindings.size(); i > 0; i--)
	{
		if (k->KeyBindings[i - 1].isPressed)
			k->onKeyPress(k->KeyBindings[i - 1].key);
	}
}
