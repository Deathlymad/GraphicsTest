#pragma once
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Def.h"
#include "LSystem.h"
#include "Mesh.h"

NSP_STD


class NoiseGraph //needs to be fully reminlemented 
{
public:
	NoiseGraph(unsigned dimension);
	~NoiseGraph();

	bool isFinished() { return noiseGenSystem.isFinished(); }

	future<void> getCalcYForPlane(vector<Mesh::Vertex>&);
	float get(float x, float y); //implement Operator
private:
	static LSystem::Rule rules[];
	LSystem noiseGenSystem;

	float getNoise(float x, float y, unsigned depth = 1);
	float noise(float x, float y);
};

