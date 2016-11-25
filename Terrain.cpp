#include "Terrain.h"
#include "World.h"
#include <algorithm>

NSP_GLM
//turn heightmap into vertexmap
//less memory, faster, behaves more proper. Dangerous. neighboring Terrains should be updated upon border change
Terrain::Terrain() : Mesh(),
_length(0),
_depth(0),
_xOff(0),
_zOff(0),
_updateState(-1),
_heightmap(nullptr)
{
}

void Terrain::init()
{
	lock_guard<mutex> lock(safeguard);
	if (_updateState & 1 || _updateState == -1)
		return;

	unsigned  x = 0, z = 0;
	unsigned StripLength = _length + 1;

	_vertices.clear();
	_vertices.resize((_depth + 1) * StripLength);

	_indices.clear();
	_indices.resize(_length * _depth * 6);

	unsigned indPos = 0;

	while (x <= _length)
	{
		z = 0;
		while (z <= _depth)
		{
			_vertices[(x * StripLength)+z] = Vertex(vec3(x + _xOff, 0, z + _zOff), vec2(), vec3());
			z += 1;
		}

		if (x > 0)
		{
			unsigned
				thisStart = x * StripLength,
				prevStart = thisStart - StripLength;

			//writes the Strip
			z = 1;
			while (z <= _length)
			{
				_indices[indPos] = prevStart + x - 1;
				_indices[indPos + 1] = prevStart + z;
				_indices[indPos + 2] = thisStart + z - 1;

				_indices[indPos + 3] = thisStart + z - 1;
				_indices[indPos + 4] = prevStart + z;
				_indices[indPos + 5] = thisStart + z;
				indPos += 6;
				z += 1;
			}
		}
		x += 1;
	}

	_updatePos = -1;
	Mesh::init();

	_updateState |= 1;
}

void Terrain::update(ThreadManager& mgr)
{
	if (!_heightmap)
		return;

	lock_guard<mutex> lock(safeguard);
	if ( (_updateState & 1) && !(_updateState & 2)) //generate Map Data
	{
		if (!_load())
		{
			_gen( 0, 0, _length, _depth, 0, 0);
			_save();
		}
		_updateState |= 2;
	}
	else if ((_updateState & 2) && !(_updateState & 4)) //generate Normal Data
	{
		_unVec = vector<Mesh::unnormalizedVertex>(_vertices.size()); //move to init phase
		copy(_vertices.begin(), _vertices.end(), _unVec.begin());
		for (unsigned i = 0; i < unsigned(_indices.size() / 3); i++) //generating Normal Data
		{
			unsigned ind = i * 3;
			vec3	a = normalize(_vertices[_indices[ind]].getPos() - _vertices[_indices[ind + 1]].getPos()),
					b = normalize(_vertices[_indices[ind]].getPos() - _vertices[_indices[ind + 2]].getPos());
			vec3 c = cross(a, b);
			if (c.y < 0)
				c = vec3(1, 1, 1) - c;
			_unVec[_indices[ind]]._nor.push_back(c);
			_unVec[_indices[ind + 1]]._nor.push_back(c);
			_unVec[_indices[ind + 2]]._nor.push_back(c);

		}
		_updateState |= 4;
	}
	else if ( (_updateState & 4) && !(_updateState & 8)) //update Data
	{
		vec3 StartPos = _vertices[0].getPos();
		for (unsigned i = 0; i < _unVec.size(); i++) //normalize the normals
		{
			vector<vec3> normals;
			for (vec3 n :_unVec[i]._nor)
				if (!(find(normals.begin(), normals.end(), n) != normals.end())) //every point is just registered once
					normals.push_back(n);
			vec3 nor = vec3(0, 0, 0);
			for (vec3 n : normals)
				nor += n;
			/*
			vec3 pos = _vertices[i].getPos();
			unsigned x, z;
			getVecPos(pos, x, z);
			pos.y = _heightmap[x][z];
			
			_vertices[i].setPos(pos);*/
			_vertices[i].setTexCoord(vec2(
				((float)rand() / (RAND_MAX)),
				((float)rand() / (RAND_MAX))
			));
			_vertices[i].setNormal(normalize(nor));
			
		}
		_updateState |= 8;
	}
}

void Terrain::Draw()
{
	
	if (_updatePos == -1 && _updateState & 8)
	{
		_updatePos = 0;
	}
	if (_updatePos != -1 && _updatePos != _vertices.size())
	{
		if (_vertices.size() > _updatePos + 100)
		{
			updateVertices(_updatePos, _updatePos + 100);
			_updatePos += 100;
		}
		else {
			updateVertices(_updatePos, _vertices.size() - 1);
			_updatePos = _vertices.size();
		}
	}
	
	Mesh::Draw();
}

void Terrain::setPos(int x, int z, int xOff, int zOff, float* heightmap)
{
	if (xOff != _xOff || zOff != _zOff)
	{
		_length = x;
		_depth = z;
		_heightmap = heightmap;
		_xOff = xOff;
		_zOff = zOff;
		_updateState = 0;
	}
}

bool Terrain::isPos(int xOff, int zOff)
{
	return xOff == _xOff && zOff == _zOff;
}

Terrain::~Terrain()
{
	_length = 0;
	_depth = 0;
	Mesh::~Mesh();
}

float Terrain::getRandomMult(float x, float z)
{
	float max = sqrtf(x*z);
	return (rand() % (int)floorf(max)) - floorf(max / 2);
}

void Terrain::_gen(int minX, int minZ, int maxX, int maxZ, int xMid, int zMid)
{
	if ((xMid + zMid) == 0) //ensuring generation
	{
		_heightmap[(minX * _length) + minZ] = 1;
		_heightmap[(maxX * _length) + minZ] = 1;
		_heightmap[(minX * _length) + maxZ] = 1;
		_heightmap[(maxX * _length) + maxZ] = 1;
	}

	int x = maxX - minX;
	int z = maxZ - minZ;

	if (x <= 1 && z <= 1)
		return;


	int halfX = (x % 2 ? x + 1 : x) / 2;
	int halfZ = (z % 2 ? z + 1 : z) / 2;


	int ptX = minX + halfX;
	int ptZ = minZ + halfZ;

	if (x == z) //diamond step
	{
		_heightmap[(ptX * _length) + ptZ] = 1;

		if (x % 2)
		{
			_heightmap[((ptX - 1) * _length) + ptZ] = 1;
			_heightmap[(ptX * _length) + ptZ - 1] = 1;
			_heightmap[((ptX - 1) * _length) + ptZ - 1] = 1;

			_gen(minX, ptZ, maxX, maxZ, ptX, ptZ);
			_gen(minX, minZ, maxX, ptZ - 1, ptX - 1, ptZ - 1);
			_gen(minX, minZ, ptX - 1, maxZ, ptX - 1, ptZ);
			_gen(ptX, minZ, maxX, maxZ, ptX, ptZ - 1);
		}
		else
		{
			_gen(minX, ptZ, maxX, maxZ, ptX, ptZ);
			_gen(minX, minZ, maxX, ptZ, ptX, ptZ);
			_gen(minX, minZ, ptX, maxZ, ptX, ptZ);
			_gen(ptX, minZ, maxX, maxZ, ptX, ptZ);
		}

	} //square algorithms
	else if (x > z)
	{
		if (zMid == minZ)
		{
			_heightmap[(ptX * _length) + maxZ] = 1;
			if (x % 2)
			{
				_heightmap[((ptX - 1) * _length) + maxZ] = 1;

				if ((ptX != maxX) && (ptZ != maxZ))
				{
					_gen(minX, minZ, ptX - 1, maxZ, ptX - 1, maxZ);
					_gen(ptX, minZ, maxX, maxZ, ptX, maxZ);
				}
			}
			else
			{
				if ((ptX != maxX) && (ptZ != maxZ))
				{
					_gen(minX, minZ, ptX, maxZ, ptX, maxZ);
					_gen(ptX, minZ, maxX, maxZ, ptX, maxZ);
				}
			}
		}
		else
		{
			_heightmap[(ptX * _length) + minZ] = 1;
			if (x % 2)
			{
				_heightmap[((ptX - 1) * _length) + minZ] = 1;

				if ((ptX != maxX) && (ptZ != maxZ))
				{
					_gen(minX, minZ, ptX - 1, maxZ, ptX - 1, minZ);
					_gen(ptX, minZ, maxX, maxZ, ptX, minZ);
				}
			}
			else
			{
				if ((ptX != maxX) && (ptZ != maxZ))
				{
					_gen(minX, minZ, ptX, maxX, ptX, minZ);
					_gen(ptX, minZ, maxX, maxX, ptX, minZ);
				}
			}
		}
	}
	else
	{
		if (xMid == minX)
		{
			_heightmap[(maxX * _length) + ptZ] = 1;
			if (z % 2)
			{
				_heightmap[(maxX * _length) + ptZ - 1] = 1;

				if ((ptX != maxX) && (ptZ != maxZ))
				{
					_gen(minX, minZ, maxX, ptZ - 1, maxX, ptZ - 1);
					_gen(minX, ptZ, maxX, maxZ, maxX, ptZ);
				}
			}
			else
			{
				if ((ptX != maxX) && (ptZ != maxZ))
				{
					_gen(minX, minZ, maxX, ptZ, maxX, ptZ);
					_gen(minX, ptZ, maxX, maxZ, maxX, ptZ);
				}
			}
		}
		else
		{
			_heightmap[(minX * _length) + ptZ] = 1;
			if (z % 2)
			{
				_heightmap[(minX * _length) + ptZ - 1] = 1;

				if ((ptX != minX) && (ptZ != minZ))
				{
					_gen(minX, minZ, maxX, ptZ - 1, minX, ptZ - 1);
					_gen(minX, ptZ, maxX, maxZ, minX, ptZ);
				}
			}
			else
			{
				if ((ptX != minX) && (ptZ != minZ))
				{
					_gen(minX, minZ, maxX, ptZ, minX, ptZ);
					_gen(minX, ptZ, maxX, maxZ, minX, ptZ);
				}
			}
		}
	}
}

void Terrain::_save()
{
	if (_heightmap[0] != 0xDEAD) //invalid data
		return;

}

bool Terrain::_load()
{
	if ( _heightmap[0] != 0xDEAD) //already loaded
		return false;
	return false;
}

void Terrain::getVecPos(vec3 & pos, unsigned & x, unsigned & z)
{
	int X = (int)floorf(pos.x);
	int Z = (int)floorf(pos.z);
	int originX = X - (X % _length);
	int originZ = Z - (Z % _depth);
	x = X - originX;
	z = Z - originZ;
}
