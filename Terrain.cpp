#include "Terrain.h"

Terrain::Terrain(Camera* ref) : Mesh(), _player(ref), oldOff(0, 0, 0), hexAmtPerLine(50), hexLines(50), initialized(false)
{}

void Terrain::init()
{
	if (initialized)
		return;
	float _sinD = sinf(1.0472f); //60 degrees in Radiens
	float _cosD = cosf(1.0472f);

	_vertices.clear();
	_indices.clear();

	unsigned i = 0, j; //running Var
	float offset = 0;
	while (i < hexLines)
	{
		j = 0;

		float vertOff = 0;
		while (j <= hexAmtPerLine)
		{
			_vertices.push_back(getVertex(offset - 0.5, vertOff));
			vertOff += 2 * _sinD;
			j++;
		}

		j = 0;

		_vertices.push_back(getVertex(offset + _cosD,0));
		_vertices.push_back(getVertex(offset + 1 + _cosD,0));
		
		while (j < hexAmtPerLine * 2) // generates vertices
		{
			size_t size = _vertices.size();
			//													   _
			//going from 3 to  2 vertices in a line (generating  /| |\ )
			if (j % 2)
			{
				//getting the three previous Vertices
				vec3
					left = _vertices[size - 3].getPos();
				vec3
					middle = _vertices[size - 2].getPos();
				vec3
					right = _vertices[size - 1].getPos();

				float off = left.z + _sinD;
				_vertices.push_back(getVertex(left.x + _cosD, off)); //left, size
				_vertices.push_back(getVertex(right.x - _cosD, off));//right, size + 1

				_indices.push_back(size - 3 - (j * 2) - hexAmtPerLine);
				_indices.push_back(size - 3);
				_indices.push_back(size); //triangle for the "implied" space between two strips

				_indices.push_back(size - 3);
				_indices.push_back(size - 2);
				_indices.push_back(size); //first triangle

				_indices.push_back(size);
				_indices.push_back(size - 2);
				_indices.push_back(size + 1); //second triangle

				_indices.push_back(size + 1);
				_indices.push_back(size - 2);
				_indices.push_back(size - 1); //third triangle
			}
			//going from 2 to  3 vertives in a line (generating  \|_|/ )
			else
			{
				//getting the two previous Vertices
				vec3
					left = _vertices[size - 2].getPos(); // use points of previous line
				vec3
					right = _vertices[size - 1].getPos();
				float off = left.z + _sinD;

				// use points of previous line
				_vertices.push_back(getVertex(left.x - _cosD, off)); //left, size
				_vertices.push_back(getVertex(right.x - _cosD, off)); //middle, size + 1
				_vertices.push_back(getVertex(right.x + _cosD, off));//right, size  + 2

				_indices.push_back(size - 3 - (j * 2) - hexAmtPerLine);
				_indices.push_back(size - 2);
				_indices.push_back(size); //triangle for the "implied" space between two strips
				
				_indices.push_back(size - 2);
				_indices.push_back(size + 1);
				_indices.push_back(size); //first triangle

				_indices.push_back(size - 1);
				_indices.push_back(size + 1);
				_indices.push_back(size - 2); //second triangle

				_indices.push_back(size + 2);
				_indices.push_back(size + 1);
				_indices.push_back(size - 1); //third triangle
			}
			j += 1;
		}
		offset += 1 + 2 * _cosD;

		i += 1;
	}
	Mesh::init();
	initialized = true;
}

void Terrain::update()
{

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
	return sin(x/100) * cos(z/100);// noise(x / 100, sinf(x / 100) * cos(z / 100), z / 100);
}

