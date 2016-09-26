#include "Terrain.h"
#include "World.h"
#include <algorithm>

NSP_GLM

Terrain::Terrain(float maxDif, float xOff, float zOff, unsigned xSize, unsigned zSize, float** heightmap) : Mesh(),
_length(xSize),
_depth(zSize),
_xOff(xOff),
_zOff(zOff),
_updateState(0)
{
	_heightmap = (float**)malloc(sizeof(float*) * (xSize + 1));
	memcpy(&_heightmap[0], &heightmap[0], (xSize + 1) * sizeof(float*));
}

Terrain::Terrain(const Terrain &other) : Mesh(other), 
_length(other._length),
_depth(other._depth),
_xOff(other._xOff),
_zOff(other._zOff),
_updateState(0)
{
	_heightmap = (float**)malloc(sizeof(float*) * (_length + 1));
	memcpy(&_heightmap[0], &(other._heightmap[0]), (_length + 1) * sizeof(float*));
}

Terrain::Terrain() : Mesh(),
_length(0),
_depth(0),
_xOff(0),
_zOff(0),
_updateState(0)
{
}

void Terrain::init()
{
	safeguard.lock();
	if (_updateState & 1)
		return;
	_updateState |= 1;
	safeguard.unlock();

	unsigned  x, y = 0;
	unsigned StripLength = _length + 1;
	vector<Mesh::Vertex> _vec( (_depth + 1) * (StripLength));
	vector<unsigned int> _ind((_length) * (_depth) * 6);
	unsigned indPos = 0;
	while (y <= _depth)
	{
		x = 0;
		while (x <= _length)
		{
			_vec[(y)*(StripLength)+x] = Vertex(vec3(x + _xOff, 0, y + _zOff), vec2(), vec3());
			x += 1;
		}

		if (y > 0)
		{
			unsigned
				thisStart = y * (StripLength),
				prevStart = (y - 1) * (StripLength);

			//writes the Strip
			x = 1;
			while (x <= _length)
			{
				_ind[indPos] = prevStart + x - 1;
				_ind[indPos + 1] = prevStart + x;
				_ind[indPos + 2] = thisStart + x - 1;

				_ind[indPos + 3] = thisStart + x - 1;
				_ind[indPos + 4] = prevStart + x;
				_ind[indPos + 5] = thisStart + x;
				indPos += 6;
				x += 1;
			}
		}
		y += 1;
	}

	_vertices = move(_vec);
	_indices = move(_ind);
	_updatePos = -1;
	Mesh::init();
}

void Terrain::update(ThreadManager* mgr)
{
	if ( (_updateState & 1) && !(_updateState & 2)) //generate Map Data
	{
		if (!_load())
		{
			_gen( 0, 0, (float)_length, (float)_depth, 0.0f, 0.0f);
			_save();
		}
		_updateState |= 2;
	}
	else if (!(_updateState & 4)) //generate Normal Data
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
			
			vec3 pos = _vertices[i].getPos();
			unsigned x, z;
			getVecPos(pos, x, z);
			pos.y = _heightmap[x][z];
			
			_vertices[i].setPos(pos);
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

void Terrain::setPos(float xOff, float zOff, float ** heightmap)
{
	if (xOff != _xOff || zOff != _zOff)
	{
		_heightmap = heightmap;
		_xOff = xOff;
		_zOff = zOff;
		_updateState = heightmap[0][0] != 0xDEAD;
	}
}

bool Terrain::isPos(float xOff, float zOff)
{
	return xOff == _xOff && zOff == _zOff;
}

Terrain & Terrain::operator=(const Terrain & other)
{
	Mesh::operator=(other);
	_length = other._length;
	_depth = other._depth;
	_xOff = other._xOff;
	_zOff = other._zOff;

	_heightmap = (float**)malloc(sizeof(float) * _length);
	memcpy(&_heightmap[0],&(other._heightmap[0]), sizeof(float*) * _length);

	return *this;
}

Terrain::~Terrain()
{
	_length = 0;
	_depth = 0;
	delete[] _heightmap;
	Mesh::~Mesh();
}

float Terrain::getRandomMult(float x, float z)
{
	float max = sqrtf(x*z);
	return (rand() % (int)floorf(max)) - floorf(max / 2);
}

void Terrain::_gen(float xOff, float zOff, float x, float z, float xMid, float zMid)
{
	if (!xMid && !zMid) //ensuing generation
	{
		_heightmap[(int)floorf(xOff)][(int)floorf(zOff)] = getRandomMult(x, z);
		_heightmap[(int)floorf(xOff + x)][(int)floorf(zOff)] = getRandomMult(x, z);
		_heightmap[(int)floorf(xOff)][(int)floorf(zOff + z)] = getRandomMult(x, z);
		_heightmap[(int)floorf(xOff + x)][(int)floorf(zOff + z)] = getRandomMult(x, z);
	}

	if (x / 2 < 1 || z / 2 < 1)
		return;

	float halfX = ceil(x / 2);
	float halfZ = ceil(z / 2);


	if (x == z) //diamond step
	{
		float neightborSum = sqrt(_heightmap[(int)floor(xOff)][(int)floor(zOff)] *
								_heightmap[(int)floor(xOff + x)][(int)floor(zOff)] *
								_heightmap[(int)floor(xOff)][(int)floor(zOff + z)] *
								_heightmap[(int)floor(xOff + x)][(int)floor(zOff + z)]);
		float height = sqrt(neightborSum * getRandomMult(halfX, halfZ));


		_heightmap[(int)floor(xOff + halfX)][(int)floor(zOff + halfZ)] = height;
		
		if ((int(x) % 2) && (int(z) % 2))
		{
			_heightmap[(int)floor(xOff + halfX) - 1][(int)floor(zOff + halfZ)] = height;
			_heightmap[(int)floor(xOff + halfX)][(int)floor(zOff + halfZ - 1)] = height;
			_heightmap[(int)floor(xOff + halfX - 1)][(int)floor(zOff + halfZ - 1)] = height;

			_gen(xOff, zOff + halfZ, x, halfZ, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff, x, halfZ - 1, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff, halfX - 1, z, xOff + halfX, zOff + halfZ);
			_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + halfZ);
		}
		
		else if (int(z) % 2)
		{
			_heightmap[(int)floor(xOff + halfX)][(int)floor(zOff + halfZ - 1)] = height;

			_gen(xOff, zOff, x, halfZ, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff, halfX - 1, z, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff + halfZ, x, halfZ, xOff + halfX, zOff + halfZ);
			_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + halfZ);
		}
		else if (int(x) % 2)
		{
			_heightmap[(int)floor(xOff + halfX) - 1][(int)floor(zOff + halfZ)] = height;

			_gen(xOff, zOff, x, halfZ - 1, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff, halfX, z, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff + halfZ, x, halfZ, xOff + halfX, zOff + halfZ);
			_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + halfZ);
		}
		else
		{
			_gen( xOff, zOff, x, halfZ, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff, halfX, z, xOff + halfX, zOff + halfZ);
			_gen(xOff, zOff + halfZ, x, halfZ, xOff + halfX, zOff + halfZ);
			_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + halfZ);
		}

	} //square algorithms
	else if (int(x) % int(z) < 2)
	{
		if (x > z)
		{
			if (zMid == zOff)
			{
				float neightborSum = sqrt(_heightmap[(int)floor(xMid)][(int)floor(zOff)] *
					_heightmap[(int)floor(xOff + x)][(int)floor(zOff + z)] *
					_heightmap[(int)floor(xOff)][(int)floor(zOff + z)]);
				float height = sqrt(neightborSum * getRandomMult(halfX, zOff + z));


				_heightmap[(int)floor(xOff + halfX)][(int)floor(zOff + z)] = height;
				if (int(x) % 2)
				{
					_heightmap[(int)floor(xOff + halfX - 1)][(int)floor(zOff + z)] = height;
					_gen(xOff, zOff, halfX - 1, z, xOff + halfX, zOff + z);
					_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + z);
				}
				else
				{
					_gen(xOff, zOff, halfX, z, xOff + halfX, zOff + z);
					_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + z);
				}
			}
			else
			{
				float neightborSum = sqrt(_heightmap[(int)floor(xMid)][(int)floor(zOff + z)] *
					_heightmap[(int)floor(xOff + x)][(int)floor(zOff)] *
					_heightmap[(int)floor(xOff)][(int)floor(zOff)]);
				float height = sqrt(neightborSum * getRandomMult(halfX, zOff));


				_heightmap[(int)floor(xOff + halfX)][(int)floor(zOff)] = height;
				if (int(x) % 2)
				{
					_heightmap[(int)floor(xOff + halfX - 1)][(int)floor(zOff)] = height;
					_gen(xOff, zOff, halfX - 1, z, xOff + halfX - 1, zOff + z);
					_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + z);
				}
				else
				{
					_gen(xOff, zOff, halfX, z, xOff + halfX, zOff + z);
					_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + z);
				}
			}

		}
		else
		{
			if (xMid == xOff)
			{
				float neightborSum = sqrt(_heightmap[(int)floor(xMid)][(int)floor(zOff + halfZ)] *
					_heightmap[(int)floor(xOff + x)][(int)floor(zOff + z)] *
					_heightmap[(int)floor(xOff + x)][(int)floor(zOff)]);
				float height = sqrt(neightborSum * getRandomMult(xOff + x, zOff + halfZ));

				_heightmap[(int)floor(xOff + x)][(int)floor(zOff + halfZ)] = height;
				if (int(x) % 2)
				{
					_heightmap[(int)floor(xOff + halfX - 1)][(int)floor(zOff + z)] = height;
					_gen(xOff, zOff, halfX - 1, z, xOff + halfX - 1, zOff + z);
					_gen(xOff + halfX - 1, zOff, halfX, z, xOff + halfX, zOff + z);
				}
				else
				{
					_gen(xOff, zOff, halfX, z, xOff + halfX, zOff + z);
					_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + z);
				}
			}
			else
			{
				float neightborSum = sqrt(_heightmap[(int)floor(xMid)][(int)floor(zOff + halfZ)] *
					_heightmap[(int)floor(xOff)][(int)floor(zOff + z)] *
					_heightmap[(int)floor(xOff)][(int)floor(zOff)]);
				float height = sqrt(neightborSum * getRandomMult(xOff, zOff + halfZ));

				_heightmap[(int)floor(xOff)][(int)floor(zOff + halfZ)] = height;
				if (int(x) % 2)
				{
					_heightmap[(int)floor(xOff)][(int)floor(zOff + halfZ - 1)] = height;
					_gen(xOff, zOff, halfX - 1, z, xOff + halfX - 1, zOff + z);
					_gen(xOff + halfX - 1, zOff, halfX, z, xOff + halfX, zOff + z);
				}
				else
				{
					_gen(xOff, zOff, halfX, z, xOff + halfX, zOff + z);
					_gen(xOff + halfX, zOff, halfX, z, xOff + halfX, zOff + z);
				}
			}
		}
	}
}

void Terrain::_save()
{
	if (_heightmap[0][0] != 0xDEAD) //invalid data
		return;

}

bool Terrain::_load()
{
	if (_heightmap[0][0] != 0xDEAD) //already loaded
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
