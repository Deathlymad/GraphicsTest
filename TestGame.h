#pragma once
#include "Game.h"
#include "ModelRenderer.h"


class TestGame :
	public Game
{
public:
	TestGame();
	
	~TestGame();
private:
	PointLight pt;
	DirectionalLight Dir;

	Material m;
	ModelRenderer obj;
};

