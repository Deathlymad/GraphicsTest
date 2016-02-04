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

LoopedThreadServer KeyMap::_keyTickServer(1);

KeyMap::KeyMap(Screen* s) : InputHandler(s), _keyTickClient( [this] {updateKeyMap(this); })
{
	_keyTickServer.addThreadClient(&_keyTickClient);
}

KeyMap::KeyMap(KeyMap & k): InputHandler(k), KeyBindings(k.KeyBindings), _keyTickClient( [this] {updateKeyMap(this); })
{
	_keyTickServer.addThreadClient(&_keyTickClient);
}

KeyMap::KeyMap() : InputHandler(), _keyTickClient( [this] {updateKeyMap(this); })
{
	_keyTickServer.addThreadClient(&_keyTickClient);
}

void KeyMap::addKeyBind( unsigned short key, function<void(unsigned short, KeyState)> Func, string name, int trig, unsigned char priority)
{
	KeyBind k;
	k.key = key | ((priority & 32) << 11);
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

KeyMap & KeyMap::operator=(KeyMap & k)
{
	KeyBindings = k.KeyBindings;
	return *this;
}

KeyMap::~KeyMap()
{}

void KeyMap::onKeyPress(char button, char action, char mods)
{
	for (unsigned char ind = 31; ind > 0; ind--) // takes a lot of time
	{
		unsigned short key = (button & 255) | ((mods & 7) << 8) | ((ind & 32) << 11);
		size_t pos= find(key, &KeyBindings, 0, KeyBindings.size());
		if (pos >= KeyBindings.size())
			continue;
		if (key != KeyBindings[pos].key)
			continue;
		if (action == GLFW_PRESS)
		{
			KeyBindings[pos].isPressed = true;
			if (KeyBindings[pos].trigger & ONPRESS && _activated)
			{
				KeyBindings[pos].callback(key, ONPRESS);
				continue;
			}
		}
		if (action == GLFW_RELEASE)
		{
			KeyBindings[pos].isPressed = false;
			if (KeyBindings[pos].trigger & ONRELEASE && _activated)
			{
				KeyBindings[pos].callback(key, ONRELEASE);
				continue;
			}
		}
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
