
#define PRAGMALIB
#ifndef GLEW
	#define GLEW_STATIC
	#include <GL/glew.h>
	#ifdef PRAGMALIB
		#pragma comment(lib, "glew32s.lib")
		#pragma comment (lib, "OpenGL32.lib")
		#pragma comment(lib, "GLU32.lib")
	#endif
	#undef __glewActiveTexture
	#undef __glewGenerateMipmap
#endif

#include "Mesh.h"
#include <fstream>

Mesh::Mesh() : _vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _vao( 3, 2, 3)
{
}

Mesh::Mesh ( string file) : _vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _vao(3)
{
	_path = file;
}

Mesh::Mesh( vector<Vertex>& vec, unsigned char bitset) : _vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _vao(bitset)
{
	vector<unsigned int> i = vector<unsigned int>({ 0,1,2,2,1,3 });//2D indices, might need additions later for other sizes
	_glDownload( vec, i);
}

Mesh::Mesh( vector<Vertex> &vec, vector < unsigned int> &i, unsigned char bitset) : _vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), _vao(bitset)
{
	_glDownload( vec, i);
}

void Mesh::load(RessourceHandler * loader)
{
	if (_path != "")
	{
		_loadReq = loader->getRessource<Mesh>(_path, this);
	}
}

void Mesh::load(ifstream &f)
{
	vector<string> temp;
	if (f.is_open())
	{
		string Line = "";
		while (getline(f, Line))
		{
			temp.push_back(Line);
		}
	}
	_load(temp);
}

void Mesh::init()
{
	if (_loadReq->get() != this)
		*this = *(_loadReq->get());
	_glDownload(getNormalVertices(_vertices), _indices); //delete?
}

void Mesh::initGL( unsigned char flag)
{
	if (flag == 0)
		return;
	if (flag & 2)
	{
		glGenBuffers( 1, _vbo.get());
	}
	if (flag & 1)
	{
		glGenBuffers(1, _ibo.get());
	}
	_vao.createVertexArray();
}

void Mesh::_load(vector<string> vec)
{
	vector<vec3> n;
	vector<vec2> t;
	unsigned char bitset = 3;

	vector<string> temp;

	for (unsigned int i = 0; i<vec.size(); i++)
	{
		temp.clear();
		if ((vec[i].find_first_of('o') != string::npos)) continue; //Object typname irrelevant
		if ((vec[i].find_first_of('m') != string::npos)) continue; //Material Data may be used someday but not now
		if ((vec[i].find_first_of('s') != string::npos)) continue; //some other data
		if ((vec[i].find_first_of('#') != string::npos) || vec[i].size() < 3) continue; //Kommentarzeilen werden KOMPLETT ignoriert, auch wenn Kommentar am Ende steht! oder Leerzeilen oder Zeile hat weniger als 3 Zeichen => bestimmt sinnlos
		io::strsep(temp, vec[i]); //vec[i] in teile zerlegen
		if (temp[0].compare("v") == 0)
		{
			_vertices.push_back(unnormalizedVertex(Vertex(vec3(stof(temp[1]), stof(temp[2]), stof(temp[3])), vec2(-1.0, -1.0), vec3(0, 0, 0)), vector<vec3>())); //writes default Data
			continue;
		}

		if (temp[0].compare("vt") == 0)
		{
			t.push_back(vec2(stof(temp[1]), stof(temp[2])));
			bitset |= VertexArrayObject::genBitset(0, 2, 0);
			continue;
		}

		if (temp[0].compare("vn") == 0)
		{
			n.push_back(vec3(stof(temp[1]), stof(temp[2]), stof(temp[3])));
			bitset |= VertexArrayObject::genBitset(0, 0, 3);
			continue;
		}

		if (temp[0].compare("f") == 0) // needs to have a check wether the face ids are seperated by space or by slash
		{
			vector<string> s;

			for (unsigned int i = 1; i < temp.size(); i++)
			{
				io::strsep(s, temp[i], '/');
				_indices.push_back(stoi(s[0]) - 1);
				if (!t.empty()) //might search for nonexistend data, also this might not be applicable all the time
					_vertices[stoi(s[0]) - 1]._v.setTexCoord(t[stoi(s[1]) - 1]);
				if (!n.empty())
					_vertices[stoi(s[0]) - 1]._nor.push_back(n[stoi(s[2]) - 1]); // need to renormalize
			}
			continue;
		}
	}
	_vao.setBitset(bitset);
}

vector<Mesh::Vertex> Mesh::getNormalVertices(vector<unnormalizedVertex> vn)
{
	vector<Vertex> _v;
	for (unnormalizedVertex v : vn)
	{
		vector<vec3> normals;
		for (vec3 n : v._nor)
		{
			if (!(find(normals.begin(), normals.end(), n) != normals.end()))
				normals.push_back(n);
		}
		vec3 nor = vec3(0, 0, 0);
		for (vec3 n : normals)
		{
			nor += n;
		}
		_v.push_back(v._v);
		_v[_v.size() - 1].setNormal(normalize(nor));
	}
	return _v;
}

void Mesh::_glDownload(vector<Vertex>& v, vector < unsigned int>& i)
{
	_VerticesCount = i.size();
	initGL(!glIsBuffer(*(_vbo.get())) << 1 | !glIsBuffer(*(_ibo.get())));

	if (v.size() == 0)
		return;


	vector<float> temp;
	for (unsigned int i = 0; i < v.size(); i++)
	{
		float* d = v[i].getData();
		if (_vao.isVec())
			temp.insert(temp.end(), &d[0], &d[_vao.isVec()]);
		if (_vao.isTex())
			temp.insert(temp.end(), &d[3], &d[3 + _vao.isTex()]);
		if (_vao.isNor())
			temp.insert(temp.end(), &d[6], &d[6 + _vao.isNor()]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, *(_vbo.get())); //contains Vertices
	glBufferData(GL_ARRAY_BUFFER, temp.size() * 4, temp.data(), GL_STATIC_DRAW);
	temp.~vector();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(_ibo.get()));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), i.data(), GL_STATIC_DRAW);
}

void Mesh::deleteBuffer(GLuint * buf)
{
	if (glIsBuffer(*buf))
		glDeleteBuffers(1, buf);
}

void Mesh::Draw()
{
	_vao.bindVertexArray();
	glBindBuffer(GL_ARRAY_BUFFER, *(_vbo.get()));
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *(_ibo.get()));

	glDrawElements(GL_TRIANGLES, _VerticesCount, GL_UNSIGNED_INT, 0);
	//Wireframe Shader
	//glDrawElements( GL_LINE_STRIP, _VerticesCount, GL_UNSIGNED_INT, 0);  //for debug purposes

	_vao.disableVAO();
}

Mesh::~Mesh(void)
{
}

void Mesh::VertexArrayObject::createVertexArray()
{
	GLuint temp = -1;
	if ( !_vao.get() || !glIsVertexArray(*(_vao.get())))
	{
		glGenVertexArrays(1, &temp);
		_vao.set(new GLuint(temp));
		glBindVertexArray( *(_vao.get()));

		if (isVec())
			enableVec();
		if (isTex())
			enableTex();
		if (isNor())
			enableNor();

		disableVAO();
	}
}

void Mesh::VertexArrayObject::bindVertexArray()
{
	glBindVertexArray( *(_vao.get()));

	if (isVec())
		enableVec();
	if (isTex())
		enableTex();
	if (isNor())
		enableNor();
}

void Mesh::VertexArrayObject::disableVAO()
{
	if (isVec())
		disableVec();
	if (isTex())
		disableTex();
	if (isNor())
		disableNor();
}

Mesh::VertexArrayObject::~VertexArrayObject()
{
}

void Mesh::VertexArrayObject::enableVec()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0 /*Vertex Attribute Layout Location*/, isVec() /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, getSize() /*stride*/, 0 /*offset*/); //Pos
}

void Mesh::VertexArrayObject::disableVec()
{
	glDisableVertexAttribArray(0);
}

void Mesh::VertexArrayObject::enableTex()
{
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1 /*Vertex Attribute Layout Location*/, isTex() /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, getSize() /*stride*/, (void*) (isVec() * 4) /*offset*/); //Tex
}

void Mesh::VertexArrayObject::disableTex()
{
	glDisableVertexAttribArray(1);
}

void Mesh::VertexArrayObject::enableNor()
{
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2 /*Vertex Attribute Layout Location*/, isNor() /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, getSize() /*stride*/, (void*) ((isVec() + isTex()) * 4) /*offset*/); //Nor
}

void Mesh::VertexArrayObject::disableNor()
{
	glDisableVertexAttribArray(2);
}
