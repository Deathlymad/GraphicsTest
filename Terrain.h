#pragma once
#include "Mesh.h"
#include "Camera.h"
class Terrain :
	public Mesh
{
public:
	Terrain(Camera* ref);

	virtual void init();
	void update();

	~Terrain();
private:
	bool initialized;

	unsigned hexAmtPerLine;
	unsigned hexLines;

	Vertex getVertex(float x, float z);
	float getHeight(float x, float z);

	Camera* _player;
	vec3 oldOff;

};

