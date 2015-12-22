#pragma once
#include "Game.h"
#include "ModelRenderer.h"
#include "Transformation.h"

class TestGame :
	public Game
{
public:
	TestGame();
	
	~TestGame();
private:
	PointLight pt;
	DirectionalLight Dir;

	Transformation trans;
	Material m;
	ModelRenderer obj;
};

