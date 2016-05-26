#include "Terrain.h"

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

Terrain::Terrain(Terrain &other) : Mesh(other), 
oldOff(other.oldOff), 
length(other.length),
depth(other.depth),
initialized(other.initialized), 
_generator(other._generator), 
heightRequested(other.heightRequested), 
updatePos(other.updatePos), 
genNormals(other.genNormals)
{
}

void Terrain::init()
{
	if (initialized)
		return;

	float _sinD = sinf(1.0472f);

	unsigned  x = 0, y = 0;
	bool toggle = false;
	while (y <= depth)
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
			unsigned cur = floorf(y * PtsPerLine - floorf(y / 2)), prev = floorf((y - 1) * PtsPerLine - floorf((y - 1) / 2));
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
	if (_generator.isFinished())
	{
		if (!heightRequested)
		{
			heightRequest = _generator.getCalcYForPlane(_vertices);
			async([this] {
				for (Vertex& v : _vertices)
					v.setTexCoord(vec2(
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
			updatePos++;
		}
		if (!genNormals && heightRequest.wait_for(seconds(0)) == future_status::ready) //normals require height data
		{
			normalRequest = async([this] {
				vector<Mesh::unnormalizedVertex> _unVec = vector<Mesh::unnormalizedVertex>(_vertices.size());
				for (unsigned i = 0; i < _unVec.size(); i++) //copy data
					_unVec[i] = _vertices[i];

				for (unsigned i = 0; i < floorf(_indices.size() / 3); i++)
				{
					unsigned ind = i * 3;
					vec3	a = _vertices[_indices[ind]].getPos() - _vertices[_indices[ind + 1]].getPos(),
							b = _vertices[_indices[ind]].getPos() - _vertices[_indices[ind + 2]].getPos();
					vec3 c = cross(a, b);
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

Terrain::~Terrain()
{
}

Mesh::Vertex Terrain::getVertex(float x, float z)
{
	return Vertex(vec3(x, getHeight(x, z), z), vec2(0, 0));
}

float Terrain::getHeight(float x, float z)
{
	return 0;
}