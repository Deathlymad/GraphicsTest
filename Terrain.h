#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "NoiseGraph.h"
class Terrain :
	public Mesh
{
public:
	Terrain(NoiseGraph& generator, float xOff, float yOff, unsigned xSize, unsigned ySize);
	Terrain(Terrain&);

	virtual void init();
	virtual void Draw();

	~Terrain();
private:
	bool initialized;

	unsigned length;
	unsigned depth;
	float _xOff, _yOff;

	NoiseGraph& _generator;
	future<void> heightRequest;
	future<void> normalRequest;
	bool heightRequested, genNormals; //merge to integral value
	unsigned updatePos;


	Vertex getVertex(float x, float z);
	float getHeight(float x, float z);

	vec3 oldOff;

};

