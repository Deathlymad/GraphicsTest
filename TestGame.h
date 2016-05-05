#pragma once
#include "Game.h"
#include "ModelRenderer.h"
#include "Transformation.h"
#include "Light.h"
#include "Terrain.h"

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

	Terrain _testField;
	ModelRenderer _fieldRenderer;
};