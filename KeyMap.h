#include <map>
#include <vector>
#include <functional>

struct GLFWwindow;

#pragma once
class KeyMap
{
public:
	KeyMap();

	void addKeyBind( unsigned short key, std::function<void()> Func);

	void onKeyPress( unsigned short);

	~KeyMap();

private:
	std::map<unsigned short, std::vector<std::function<void()>>> KeyBindings; // short in binary 00000 3 bit modifier Key (Alt, Shift, Ctrl) 8 bit key

	static std::vector<KeyMap*> KeyMaps;
};