#include "Terrain.h"
#include <algorithm>

NSP_GLM

Terrain::Terrain(NoiseGraph& generator, float xOff, float yOff, unsigned xSize, unsigned ySize) : Mesh(),
length(xSize),
depth(ySize),
_xOff(xOff),
_yOff(yOff),
_initState(0),
_generator(generator),
genPos(0),
updatePos(0), 
genNormals(false)
{}

Terrain::Terrain(const Terrain &other) : Mesh(other), 
length(other.length),
depth(other.depth),
_xOff(other._xOff),
_yOff(other._yOff),
_initState(other._initState),
_generator(other._generator), 
genPos(other.genPos),
updatePos(other.updatePos), 
genNormals(other.genNormals)
{
}

Terrain::Terrain() : Mesh(),
length(0),
depth(0),
_xOff(0),
_yOff(0),
_initState(0),
_generator(NoiseGraph()),
genPos(0),
updatePos(0),
genNormals(false)
{
}

void Terrain::init()
{
	if (isInitialized() || isGenRunning())
		return;
	_initState = _initState | 2;
	float _sinD = 1.0f;

	unsigned  x = 0, y = 0;
	bool toggle = false;
	vector<Mesh::Vertex> _vec;
	vector<unsigned int> _ind;
	while (y * _sinD <= depth)
	{
		x = 0;
		float offset = toggle ? 0.0f : 0.5f;
		if (offset != 0.0f)
			_vec.push_back(Vertex(vec3(_xOff, 0, y * _sinD + _yOff), vec2()));
		while (x < length)
		{
			if (_initState == 0)
				break;
			_vec.push_back(Vertex(vec3(offset + x + _xOff, 0, y * _sinD + _yOff), vec2()));
			x+=1;
		}
		_vec.push_back(Vertex(vec3(ceil(x) + _xOff, 0, y * _sinD + _yOff), vec2()));

		if (y > 0)
		{
			float PtsPerLine = length + 2.0f;
			unsigned	cur = unsigned(float(y) * PtsPerLine - floorf((float(y) / 2.0f))), 
						prev = unsigned(floorf(float(y - 1) * PtsPerLine - floorf(float(y - 1) / 2.0f)));
			unsigned linePos = y % 2 ? cur : prev, prevLinePos = y % 2 ? prev : cur;
			
			//writes first Triangle
			_ind.push_back(linePos);
			_ind.push_back(prevLinePos);
			_ind.push_back(prevLinePos + 1);

			//writes the rest of the Strip
			for (unsigned i = 0; i < PtsPerLine - 2; i++)
			{
				_ind.push_back(linePos + i);
				_ind.push_back(prevLinePos + (i + 1));
				_ind.push_back(linePos + (i + 1));

				_ind.push_back(linePos + (i + 1));
				_ind.push_back(prevLinePos + (i + 1));
				_ind.push_back(prevLinePos + (i + 2));
			}
		}
		y+=1;
		if (toggle)
			toggle = false;
		else
			toggle = true;

	if (_initState == 0)
		break;
	}
	_vertices = move(_vec);
	_indices = move(_ind);
	_initState = _initState & ( 8 | 4);
	_initState = _initState | 1;
}

void Terrain::update(ThreadManager* mgr)
{
	if (length == 0 || depth == 0 || !isInitialized())
		return;
	
	if (_generator.isFinished() && isInitialized() && !genNormals && genPos == 0 && (_initState & 8) == 0)
	{
		mgr->addTask(
			[this] {

			vector<Mesh::unnormalizedVertex> _unVec = vector<Mesh::unnormalizedVertex>(_vertices.size());
			for (unsigned i = 0; i < _unVec.size(); i++) //copy data
				_unVec[i] = _vertices[i];

			for (unsigned i = 0; i < unsigned(_indices.size() / 3); i++)
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

				if (_initState == 0)
					break;
			}
			for (unsigned i = 0; i < _unVec.size(); i++) //normalize the normals
			{
				vector<vec3> normals;
				for (vec3 n :_unVec[i]._nor)
				{
					if (!(find(normals.begin(), normals.end(), n) != normals.end())) //evry point is just registered once
						normals.push_back(n);

					if (_initState == 0)
						break;
				}
				vec3 nor = vec3(0, 0, 0);
				for (vec3 n : normals)
				{
					nor += n;
				}
				_vertices[i].setPos(
					vec3(
						_unVec[i]._v.getPos().x,
						_generator.get(_unVec[i]._v.getPos().x, _unVec[i]._v.getPos().z),
						_unVec[i]._v.getPos().z
					)
				);
				_vertices[i].setTexCoord(vec2(
					((float)rand() / (RAND_MAX)),
					((float)rand() / (RAND_MAX))
				));
				_vertices[i].setNormal(normalize(nor));
				genPos = i;

				if (_initState == 0)
					break;
			}
			genNormals = true;
			return 0;
		});
		_initState = _initState | 8;
	}
}

void Terrain::Draw()
{
	if (length == 0 || depth == 0 || !isMeshInitialized() || _vertices.empty())
		return;
	if (_vertices.size() - 1 > updatePos)
	{
		unsigned updateSteps = 100; //make auto or add option
		unsigned updateDif = genPos - updatePos;
		unsigned maxPos = _vertices.size();
		if (genPos == maxPos && maxPos - updatePos <= updateSteps)
		{
			updateVertices(updatePos, maxPos);
			updatePos = maxPos;
		}
		else if (updateDif > 0)
		{
			if (updateDif <= updateSteps)
			{
				updateVertices(updatePos, genPos);
				updatePos = genPos;
			}
			else
			{
				updateVertices(updatePos, updatePos + updateSteps);
				updatePos += updateSteps;
			}
		}
	}
	Mesh::Draw();
}

float Terrain::distToPoint(vec3 p)
{
	if (length == 0 || depth == 0)
		return 1000000;
	if ( _xOff <= p.x && p.x <= (_xOff + length) &&
		_yOff <= p.z && p.z <= (_yOff + depth))
		return 0;
	float a = _distTo(p, _xOff, _yOff)
		, b = _distTo(p, _xOff + length, _yOff)
		, c = _distTo(p, _xOff, _yOff + depth)
		, d = _distTo(p, _xOff + length, _yOff + depth);
	float t1 = (a > b) ? b : a
		, t2 = (c > d) ? d : c;
	return (t1 > t2) ? t2 : t1;
}

Terrain & Terrain::operator=(const Terrain & other)
{
	Mesh::operator=(other);
	_initState = other._initState & (4 | 2 | 1);
	_xOff = other._xOff;
	_yOff = other._yOff;
	genPos = 0;
	updatePos = 0;
	genNormals = false;

	return *this;
}

Terrain::~Terrain()
{
	length = 0;
	depth = 0;
	genPos = updatePos = _vertices.size();
	_initState = 0;
	Mesh::~Mesh();
}

float Terrain::_distTo(vec3 p, float x, float z)
{
	float _x = powf(p.x - x, 2);
	float _z = powf(p.z - z, 2);
	return sqrtf( _x + _z);
}

Mesh::Vertex Terrain::getVertex(float x, float z)
{
	return Vertex(vec3(x, getHeight(x, z), z), vec2(0, 0));
}

float Terrain::getHeight(float x, float z)
{
	return 0;
}