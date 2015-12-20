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

KeyMap::KeyMap(Screen* s) : InputHandler(s), KeyTick([] {}, [this] {updateKeyMap(this); }, 20)
{}

KeyMap::KeyMap(KeyMap & k): InputHandler(k), KeyBindings(k.KeyBindings), KeyTick([] {}, [this] {updateKeyMap(this); }, 20)
{

}

KeyMap::KeyMap() : InputHandler(), KeyTick([] {}, [this] {updateKeyMap(this); }, 20)
{
}

void KeyMap::addKeyBind( unsigned short key, function<void(unsigned short)> Func, string name)
{
	KeyBind k;
	k.key = key;
	k.callback = Func;
	k.name = name;
	k.isPressed = false;
	int newPos = find(key, &KeyBindings, 0, KeyBindings.size());
	KeyBindings.insert(KeyBindings.begin() + newPos, k);
}

void KeyMap::onKeyPress(unsigned short key)
{
	KeyBind temp = KeyBindings[find(key, &KeyBindings, 0, KeyBindings.size())];

	temp.callback(key);
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
	unsigned short key = button | (mods << 8);
	size_t pos= find(key, &KeyBindings, 0, KeyBindings.size());
	if (pos >= KeyBindings.size())
		return;
	if (key != KeyBindings[pos].key)
		return;
	if (action == GLFW_PRESS)
		KeyBindings[pos].isPressed = true;
	if (action == GLFW_RELEASE)
		KeyBindings[pos].isPressed = false;
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
	/*
	if (max > min)
	{
		int pos = floor((max - min)/2) + min;

		if (arr->size() < min)
			return -1;

		//recursion
		if ( (*arr)[pos].key < key)
			return find(key, arr, pos, max);
		else if ((*arr)[pos].key > key)
			return find(key, arr, min, pos + 1);
		else
			return pos;
	}
	else
		return min;
		*/
}

void KeyMap::updateKeyMap(KeyMap * k)
{
	vector<KeyBind>* Bindings = &(k->KeyBindings);
	for (size_t i = 0; i < Bindings->size(); i++)
	{
		if ((*Bindings)[i].isPressed)
			k->onKeyPress((*Bindings)[i].key);
	}
}
