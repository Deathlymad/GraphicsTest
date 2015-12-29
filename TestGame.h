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
	DirectionalLight Light;
	PointLight Light1;
	SpotLight Light2;

	Transformation trans;
	Material m;
	ModelRenderer obj;
};

