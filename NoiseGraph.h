#pragma once
#include <vector>

#include "Def.h"
#include "LSystem.h"

NSP_STD


class NoiseGraph
{
	const static LSystem::Rule GenRules[];

public:
	NoiseGraph(unsigned dimension);
	~NoiseGraph();

	float get(float x, float y); //implement Operator
	bool isFinished() {
		bool done = true;
		for (LSystem& system : systems)
			done = done && system.isFinished();
		return sampler.wait_for(seconds(0)) == std::future_status::ready && done;
	}

private:
	void sample();

	float getWeightedSum(vector<float>&, vector<float>&);
	float getWeightedSum(float, float, float, float);
	float getInterpolated(vector<float>& arr, float index);
	void mix(vector<float>&,vector<float>&,vector<float>&,vector<float>&);

	unsigned _dimension;

	future<void> sampler;

	vector<LSystem> systems;
	std::vector<vector<float>> graph;
};

