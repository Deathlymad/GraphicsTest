#include <functional>

#include "Def.h"

NSP_STD

#pragma once
class ResizeCallback //needs identification
{
public:
	ResizeCallback()
	{

	}

	void operator()(int width, int height)
	{
		onCallback( width, height);
	}
private:
	void onCallback(int width, int height)
	{
		return;
	}
};
#pragma once
class KeyCallback //needs identification
{
public:
	KeyCallback()
	{

	}

	void operator()(char button, char action, char mods)
	{
		onCallback( button, action, mods);
	}
private:
	void onCallback(char button, char action, char mods)
	{
		return;
	}
};
#pragma once
class MouseMoveCallback //needs identification
{
public:
	MouseMoveCallback()
	{

	}

	void operator()(double posX, double posY, double x, double y)
	{
		onCallback( posX, posY, x, y);
	}
private:
	void onCallback(double posX, double posY, double x, double y)
	{
		return;
	}
};
#pragma once
class MouseButtonCallback //needs identification
{
public:
	MouseButtonCallback()
	{

	}

	void operator()(char button, char action, char mods)
	{
		onCallback( button, action, mods);
	}
private:
	void onCallback(char button, char action, char mods)
	{
		return;
	}
};
#pragma once
class ScrollCallback //needs identification
{
public:
	ScrollCallback()
	{

	}

	void operator()(double x, double y)
	{
		onCallback(x, y);
	}
private:
	void onCallback(double x, double y)
	{
		return;
	}
};