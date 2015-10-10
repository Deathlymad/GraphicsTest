#include "KeyMap.h"

std::vector<KeyMap*> KeyMap::KeyMaps;

KeyMap::KeyMap() : KeyBindings()
{
	KeyMaps.push_back(this);
}

void KeyMap::addKeyBind( unsigned short key, std::function<void()> Func)
{
	KeyBindings[key].push_back(Func);
}

void KeyMap::onKeyPress(unsigned short key)
{
	for (KeyMap* k : KeyMaps)
	{
		std::vector<std::function<void()>> funcs = k->KeyBindings[key];

		for (std::function<void()> f : funcs)
			f();
	}
}

KeyMap::~KeyMap()
{
}
