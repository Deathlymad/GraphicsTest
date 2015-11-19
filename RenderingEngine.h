#include "ExclusiveObject.h"

#pragma once
class RenderingEngine : ThreadExclusiveObject<RenderingEngine>
{
public:
	RenderingEngine();
	~RenderingEngine();
private:
};