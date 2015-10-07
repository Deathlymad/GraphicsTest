#include <map>
#include <vector>
#include <functional>

#pragma once
class KeyMap
{
public:
	KeyMap();

	void addKeyBind( unsigned short key, std::function<void()> Func);

	void onKeyPress();

	~KeyMap();

	static KeyMap instance; //should be possibly modified for multiplay profiles
private:
	std::map<unsigned short, std::vector<std::function<void()>>> KeyBindings;
};