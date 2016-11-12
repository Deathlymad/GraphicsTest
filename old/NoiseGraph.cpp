#include "NoiseGraph.h"
#include "LSystem.h"

#include <stack>
LSystem::Rule NoiseGraph::rules[] = {
	LSystem::Rule('A', string("DGF")),
	LSystem::Rule('B', string("BLC")),
	LSystem::Rule('C', string("KDA")),
	LSystem::Rule('D', string("CJG")),

	LSystem::Rule('E', string("HAC")),
	LSystem::Rule('F', string("LDB")),
	LSystem::Rule('G', string("GHJ")),
	LSystem::Rule('H', string("FGH")),

	LSystem::Rule('I', string("ALC")),
	LSystem::Rule('J', string("FEC")),
	LSystem::Rule('K', string("EJK")),
	LSystem::Rule('L', string("ELA"))
};
NoiseGraph::NoiseGraph( ThreadManager* manager, unsigned dimension) : noiseGenSystem( LSystem('A' + rand() % 11, 2))
{
	for (LSystem::Rule rule : rules)
		noiseGenSystem.addRule(rule);
	manager->addTask([this] { return noiseGenSystem.run(); });
}

NoiseGraph::NoiseGraph() : noiseGenSystem(LSystem(0, 0))
{
}

NoiseGraph::~NoiseGraph()
{

}

void NoiseGraph::getCalcYForPlane( vector<Mesh::Vertex>& vec)
{
	for (unsigned i = 0; i< vec.size(); i++)
	{
		Mesh::Vertex& pos = vec[i];
		vec3& posRef = pos.getPos();
		pos.setPos( vec3(posRef.x, get(posRef.x, posRef.z), posRef.z));
	}
}

float NoiseGraph::get( float x, float y)
{
	return getNoise(x/20, y/20) * 5;
}

float NoiseGraph::getNoise(float x, float y, unsigned depth)
{
	if (depth > 25)
		return noise( x, y);
	return noise( x, y) + (getNoise(x * 2, y * 2, depth + 1) / powf(2.0f, (float)depth));
}

float NoiseGraph::noise(float x, float y)
{
	float res = 1;
	string str = noiseGenSystem.getResult();
	for (const char& c : str)
		switch (c)
		{
		case 'A':
			res += cosf(x);
			break;
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
			res += sinf(y);
			break;
		case 'G':
			res += sinf(res);
			break;
		case 'H':
			res += sinf(-res);
			break;

		case 'I':
			res += sinf(x) * cosf(y);
			break;
		case 'J':
			res += sinf(y) * cosf(y);
			break;
		case 'K':
			res += sinf(x) * cosf(x);
			break;
		case 'L':
			res += sinf(y) * cosf(x);
			break;
		default:
			break;
		}
	return res;
}
