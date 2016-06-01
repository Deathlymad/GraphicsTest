#include "Terrain.h"
#include <algorithm>

NSP_GLM

Terrain::Terrain(NoiseGraph& generator, float xOff, float yOff, unsigned xSize, unsigned ySize) : Mesh(),
oldOff(0, 0, 0),
length(xSize),
depth(ySize),
_xOff(xOff),
_yOff(yOff),
initialized(false), 
_generator(generator), 
heightRequested(false), 
updatePos(0), 
genNormals(false)
{}

Terrain::Terrain(const Terrain &other) : Mesh(other), 
oldOff(other.oldOff), 
length(other.length),
depth(other.depth),
_xOff(other._xOff),
_yOff(other._yOff),
initialized(other.initialized), 
_generator(other._generator), 
heightRequested(other.heightRequested), 
updatePos(other.updatePos), 
genNormals(other.genNormals)
{
}

Terrain::Terrain() : Mesh(),
oldOff(0),
length(0),
depth(0),
_xOff(0),
_yOff(0),
initialized(false),
_generator(NoiseGraph()),
heightRequested(false),
updatePos(0),
genNormals(false)
{
}

void Terrain::init()
{
	if (initialized)
		return;

	float _sinD = 1;// sinf(1.0472f);

	unsigned  x = 0, y = 0;
	bool toggle = false;
	while (y * _sinD <= depth)
	{
		x = 0.0f;
		float offset = toggle ? 0 : 0.5;
		if (offset != 0.0f)
			_vertices.push_back(Vertex(vec3(_xOff, 0, y * _sinD + _yOff), vec2()));
		while (x < length)
		{
			_vertices.push_back(Vertex(vec3(offset + x + _xOff, 0, y * _sinD + _yOff), vec2()));
			x+=1;
		}
		_vertices.push_back(Vertex(vec3(ceil(x) + _xOff, 0, y * _sinD + _yOff), vec2()));

		if (y > 0)
		{
			float PtsPerLine = length + 2;
			unsigned cur = unsigned(y * PtsPerLine - floorf(y / 2)), prev = floorf((y - 1) * PtsPerLine - floorf((y - 1) / 2));
			unsigned linePos = y % 2 ? cur : prev, prevLinePos = y % 2 ? prev : cur;
			


			//writes first Triangle
			_indices.push_back(linePos);
			_indices.push_back(prevLinePos);
			_indices.push_back(prevLinePos + 1);

			//writes the rest of the Strip
			for (unsigned i = 0; i < PtsPerLine - 2; i++)
			{
				_indices.push_back(linePos + i);
				_indices.push_back(prevLinePos + (i + 1));
				_indices.push_back(linePos + (i + 1));

				_indices.push_back(linePos + (i + 1));
				_indices.push_back(prevLinePos + (i + 1));
				_indices.push_back(prevLinePos + (i + 2));
			}
		}
		y+=1;
		if (toggle)
			toggle = false;
		else
			toggle = true;
	}
	
	
	Mesh::init();
	initialized = true;
}

void Terrain::Draw()
{
	if (length == 0 && depth == 0)
		return;
	
	if (_generator.isFinished())
	{
		if (!heightRequest.valid()  || !heightRequested)
		{
			heightRequest = _generator.getCalcYForPlane(_vertices);
			
			async([this] {
				for (unsigned i = 0; i < _vertices.size(); i++)
					_vertices[i].setTexCoord(vec2(
					((float)rand() / (RAND_MAX)),
					((float)rand() / (RAND_MAX))
					));
			});
			this_thread::sleep_for(milliseconds(10));
			heightRequested = true;
		}
		else if (updatePos < _vertices.size() && normalRequest.valid() && normalRequest.wait_for(seconds(0)) == future_status::ready)
		{
			updateVertices(updatePos, updatePos + 6);
			updatePos+=6;
		}
		if (!genNormals && heightRequest.wait_for(seconds(0)) == future_status::ready) //normals require height data
		{
			normalRequest = async([this] {
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
				}
				for (unsigned i = 0; i < _unVec.size(); i++) //normalize the normals
				{
					vector<vec3> normals;
					for (vec3 n :_unVec[i]._nor)
					{
						if (!(find(normals.begin(), normals.end(), n) != normals.end()))
							normals.push_back(n);
					}
					vec3 nor = vec3(0, 0, 0);
					for (vec3 n : normals)
					{
						nor += n;
					}
					_vertices[i].setNormal(normalize(nor));
				}
				genNormals = true;
			});
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

Terrain & Terrain::operator=(Terrain & other)
{
	Mesh::operator= (other);

	oldOff = other.oldOff;
	length = other.length;
	depth = other.depth;
	_xOff = other._xOff;
	_yOff = other._yOff;
	initialized = other.initialized;
	_generator = other._generator;
	heightRequested = other.heightRequested;
	updatePos = other.updatePos;
	genNormals = other.genNormals;

	return *this;
}

bool Terrain::operator==(vec2 pos)
{
	if (length == 0 || depth == 0)
		return false;
	return _xOff == pos.x && _yOff == pos.y;
}

Terrain::~Terrain()
{
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