#include "KeyMap.h"

KeyMap KeyMap::instance;

KeyMap::KeyMap() : KeyBindings()
{
}

void KeyMap::addKeyBind( unsigned short key, std::function<void()> Func)
{
	KeyBindings[key].push_back(Func);
}

void KeyMap::onKeyPress(unsigned short key)
{
	std::vector<std::function<void()>> funcs = KeyBindings[key];

	for (std::function<void()> f : funcs)
		f();
}

KeyMap::~KeyMap()
{
}
