#include "Terrain.h"

int Terrain::p[512];
int Terrain::permutation[] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

Terrain::Terrain(Camera* ref) : Mesh(), _player(ref), oldOff(0, 0, 0), hexAmtPerLine(50), hexLines(50), initialized(false)
{
	for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
}

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
	return noise( x, sinf(x * z) * cosf(x/z), z) * 3;
}

//Credit goes to Ken Perlin --> SIGGRAPH 2002 improved Noise

double Terrain::noise(double x, double y, double z) {
	int X = (int)floor(x) & 255,                  // FIND UNIT CUBE THAT
		Y = (int)floor(y) & 255,                  // CONTAINS POINT.
		Z = (int)floor(z) & 255;
	x -= floor(x);                                // FIND RELATIVE X,Y,Z
	y -= floor(y);                                // OF POINT IN CUBE.
	z -= floor(z);
	double u = fade(x),                                // COMPUTE FADE CURVES
		v = fade(y),                                // FOR EACH OF X,Y,Z.
		w = fade(z);
	int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
		B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

	return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),  // AND ADD
		grad(p[BA], x - 1, y, z)), // BLENDED
		lerp(u, grad(p[AB], x, y - 1, z),  // RESULTS
			grad(p[BB], x - 1, y - 1, z))),// FROM  8
		lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),  // CORNERS
			grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));
}
double Terrain::grad(int hash, double x, double y, double z) {
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
	double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
		v = h<4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}