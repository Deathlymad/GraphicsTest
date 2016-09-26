#pragma once
#include <random>

#include "Mesh.h"
#include "Camera.h"
#include "NoiseGraph.h"
#include "ThreadManager.h"

class Terrain :
	public Mesh
{
public:
	Terrain(float maxDif, float xOff, float zOff, unsigned xSize, unsigned zSize, float** heightmap);
	Terrain(const Terrain&);
	Terrain();

	virtual void init();
	virtual void update(ThreadManager*);
	virtual void Draw();

	void onUpdateMap()
	{
		_updateState = 1;
	}

	bool isInit() { return _updateState & 1; }

	void setPos(float xOff, float zOff, float** heightmap);
	bool isPos(float xOff, float zOff);

	Terrain& operator=(const Terrain& other);

	~Terrain();
private:
	float getRandomMult(float x, float z);
	void _gen(float xOff, float zOff, float x, float z, float xMid, float zMid);
	void _save();
	bool _load();

	void getVecPos(vec3& pos, unsigned& x, unsigned& z);
	
	unsigned _length;
	unsigned _depth;
	float _xOff, _zOff;
	vector<Mesh::unnormalizedVertex> _unVec;
	
	float** _heightmap;

	mutex safeguard;
	int _updateState;
	unsigned _updatePos;
};

