#include "KeyMap.h"

KeyMap KeyMap::instance;

KeyMap::KeyMap() : KeyBindings()
{
}

void KeyMap::addKeyBind( unsigned short key, std::function<void()> Func)
{
	KeyBindings[key].push_back(Func);
}


KeyMap::~KeyMap()
{
}
