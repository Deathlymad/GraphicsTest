#include "NoiseGraph.h"
#include "LSystem.h"

#include <stack>
LSystem::Rule NoiseGraph::rules[] = {
	LSystem::Rule('A', "DGF"),
	LSystem::Rule('B', "BLC"),
	LSystem::Rule('C', "KDA"),
	LSystem::Rule('D', "CJG"),

	LSystem::Rule('E', "HAC"),
	LSystem::Rule('F', "LDB"),
	LSystem::Rule('G', "GHJ"),
	LSystem::Rule('H', "FGH"),

	LSystem::Rule('I', "ALC"),
	LSystem::Rule('J', "FEC"),
	LSystem::Rule('K', "EJK"),
	LSystem::Rule('L', "ELA")
};
NoiseGraph::NoiseGraph(unsigned dimension) : noiseGenSystem( LSystem('A' + rand() % 11, 1))
{
	for (LSystem::Rule rule : rules)
		noiseGenSystem.addRule(rule);
	noiseGenSystem.start();
}

NoiseGraph::NoiseGraph() : noiseGenSystem(LSystem(0, 0))
{
}

NoiseGraph::~NoiseGraph()
{

}

future<void> NoiseGraph::getCalcYForPlane(vector<Mesh::Vertex>& vec)
{
	return async([this, &vec]
	{
		for (unsigned i = 0; i< vec.size(); i++)
		{
			Mesh::Vertex& pos = vec[i];
			vec3& posRef = pos.getPos();
			pos.setPos( vec3(posRef.x, get(posRef.x, posRef.z), posRef.z));
		}
	});
}

float NoiseGraph::get( float x, float y)
{
	return getNoise(x/20, y/20) * 5;
}

float NoiseGraph::getNoise(float x, float y, unsigned depth)
{
	if (depth > 25)
		return noise( x, y);
	return noise( x, y) + (getNoise(x * 2, y * 2, depth + 1) / powf(2, depth));
}

float NoiseGraph::noise(float x, float y) //paralellizee
{
	float res = 1;
	string str = noiseGenSystem.getResult();
	for (const char& c : str)
		switch (c)
		{
		case 'A':
			res += cosf(x);
		case 'B':
			res += cosf(y);
			break;
		case 'C':
			res += cosf(res);
			break;
		case 'D':
			res += cosf(-res);
			break;

		case 'E':
			res += sinf(x);
			break;
		case 'F':
			res *= sinf(y);
			break;
		case 'G':
			res += sinf(res);
			break;
		case 'H':
			res *= sinf(-res);
			break;

		case 'I':
			res += sinf(x) * cosf(y);
			break;
		case 'J':
			res *= sinf(y) * cosf(y);
			break;
		case 'K':
			res *= sinf(x) * cosf(x);
			break;
		case 'L':
			res *= sinf(y) * cosf(x);
			break;
		default:
			break;
		}
	return res;
}
