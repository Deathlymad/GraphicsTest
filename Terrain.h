#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "NoiseGraph.h"
class Terrain :
	public Mesh
{
public:
	Terrain(Camera* ref);

	virtual void init();
	virtual void Draw();

	~Terrain();
private:
	bool initialized;
	bool heightApplied;

	unsigned hexAmtPerLine;
	unsigned hexLines;

	NoiseGraph generator;

	Vertex getVertex(float x, float z);
	float getHeight(float x, float z);

	Camera* _player;
	vec3 oldOff;

};

