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
		if (MeshInit || !_init)
			return;
		Mesh::init();
		MeshInit = true;
	}
	virtual void init();
	virtual void update();
	virtual void Draw();

	bool isInitialized() { return _init; }
	bool isMeshInitialized() { return MeshInit; }

	bool isTerrainAt(float x, float y) { return distToPoint(vec3(x, 0, y)) == 0; }
	float distToPoint(vec3 p);

	Terrain& operator= (Terrain& other);

	~Terrain();
private:
	float _distTo(vec3 p, float x, float z);

	bool _init, MeshInit;

	unsigned length;
	unsigned depth;
	float _xOff, _yOff;

	NoiseGraph& _generator;
	future<void> heightRequest;
	future<void> normalRequest;
	bool heightRequested, genNormals; //merge to integral value
	unsigned genPos, updatePos;


	Vertex getVertex(float x, float z);
	float getHeight(float x, float z);

	vec3 oldOff;

};

