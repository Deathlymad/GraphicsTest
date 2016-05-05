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

	static int p[512], permutation[];
	double noise(double, double, double);
	double Terrain::fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	double Terrain::lerp(double t, double a, double b) { return a + t * (b - a); }
	double Terrain::grad(int hash, double x, double y, double z);
};

