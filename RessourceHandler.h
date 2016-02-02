#include <future>
#include "Clock.h"

#pragma once
class RessourceHandler
{
public:
	RessourceHandler();

	promise<char**> loadFile(string file);

	~RessourceHandler();
private:
	Clock Loader;
};

