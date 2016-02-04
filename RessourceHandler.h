#include <future>
#include <string>
#include <queue>
#include <functional>

#include "Clock.h"
#include "Util.h"

NSP_STD
NSP_UTIL

//define ressource Handler, Ressource

#pragma once
class RessourceLoader
{
public:
	RessourceLoader();

	ThreadClient* loadCode(string file, function<void(future<char**>&)> = [] {});
	ThreadClient* loadFile(string file, function<void(ifstream&)>);

	bool loading() { return fileLoader.hasTasks(); }

	~RessourceLoader();
private:
	void _load(string file, function<void(future<char**>&)>, promise<char**>*);

	ThreadServer fileLoader;
};