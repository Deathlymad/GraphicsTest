#pragma once
#include <random>

#include "Mesh.h"
#include "Camera.h"
//#include "NoiseGraph.h"
#include "ThreadManager.h"

class Terrain :
	public Mesh
{
public:
	Terrain();

	virtual void init();
	virtual void update(ThreadManager&);
	virtual void Draw();

	void onUpdateMap()
	{
		_updateState = 1;
	}

	bool isInit() { return _updateState & 1; }

	void setPos(int x, int z, int xOff, int zOff, float* heightmap);
	bool isPos(int xOff, int zOff);

	~Terrain();
private:
	float getRandomMult(float x, float z);
	void _gen(int xOff, int zOff, int x, int z, int xMid, int zMid);
	void _save();
	bool _load();

	void getVecPos(vec3& pos, unsigned& x, unsigned& z);
	
	unsigned _length;
	unsigned _depth;
	int _xOff, _zOff;
	vector<Mesh::unnormalizedVertex> _unVec;
	
	float* _heightmap;

	mutex safeguard;
	int _updateState;
	unsigned _updatePos;
};

