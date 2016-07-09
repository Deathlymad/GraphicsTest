#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "NoiseGraph.h"
class Terrain :
	public Mesh
{
public:
	Terrain(NoiseGraph& generator, float xOff, float yOff, unsigned xSize, unsigned ySize);
	Terrain(const Terrain&);
	Terrain();

	void initMesh() {
		if (isMeshInitialized() || !isInitialized())
			return;
		Mesh::init();
		_initState = _initState | 4;
	}
	virtual void init();
	virtual void update(ThreadManager*);
	virtual void Draw();

	bool isInitialized() { return (_initState & 1) != 0; }
	bool isGenRunning() { return (_initState & 2) != 0; }
	bool isMeshInitialized() { return (_initState & 4) != 0; }

	bool isTerrainAt(float x, float y) { return distToPoint(vec3(x, 0, y)) == 0; }
	float distToPoint(vec3 p);
	
	~Terrain();
private:
	float _distTo(vec3 p, float x, float z);

	int _initState;

	unsigned length;
	unsigned depth;
	float _xOff, _yOff;

	NoiseGraph& _generator;
	bool genNormals;
	unsigned genPos, updatePos;


	Vertex getVertex(float x, float z);
	float getHeight(float x, float z);

	vec3 oldOff;

};

