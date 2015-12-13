#pragma once
#include "Game.h"
class TestGame :
	public Game
{
public:
	TestGame();
	
	~TestGame();
private:
	void buildWorld();

	PointLight spot;
	DirectionalLight Dir;
};

