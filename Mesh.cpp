
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

Mesh::Mesh() : vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), vao(true, true, true)
{
}

Mesh::Mesh ( string file) : vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), vao(true, true, true)
{
	vector<Vertex> v;
	vector<unsigned int> in;
	vector<vec3> n;
	vector<vec2> t;
	
	vector<string> vec;
	vector<string> temp;
	load(vec, file);
		
	for(unsigned int i = 0; i<vec.size(); i++)
	{
		temp.clear();
		if((vec[i].find_first_of('o') != string::npos)) continue; //Object typname irrelevant
		if((vec[i].find_first_of('m') != string::npos)) continue; //Material Data may be used someday but not now
		if((vec[i].find_first_of('s') != string::npos)) continue; //some other data
		if((vec[i].find_first_of('#') != string::npos) || vec[i].size() < 3) continue; //Kommentarzeilen werden KOMPLETT ignoriert, auch wenn Kommentar am Ende steht! oder Leerzeilen oder Zeile hat weniger als 3 Zeichen => bestimmt sinnlos
		io::strsep(temp,vec[i]); //vec[i] in teile zerlegen
		if(temp[0].compare("v") == 0)
		{
			v.push_back( Vertex(vec3( stof(temp[1]), stof(temp[2]), stof(temp[3])) , vec2(-1.0, -1.0), vec3(0,0,0))); //writes default Data
			continue;
		}
		
		if(temp[0].compare("vt") == 0)
		{
			t.push_back(vec2(stof(temp[1]),stof(temp[2])));
			continue;
		}
			
		if(temp[0].compare("vn") == 0)
		{
			n.push_back(vec3(stof(temp[1]),stof(temp[2]),stof(temp[3])));
			continue;
		}
		
		if(temp[0].compare("f") == 0) // needs to have a check wether the face ids are seperated by space or by slash
		{
			vector<string> s;
			
			for (unsigned int i = 1; i < temp.size(); i++)
			{
				io::strsep( s, temp[i], '/');
				in.push_back(stoi(s[0]) - 1);
				if (t.size() > 0)
					v[stoi(s[0]) - 1].setTexCoord( t[stoi(s[1]) - 1]);
				if(n.size() > 0)
					v[stoi(s[0]) - 1].setNormal(v[stoi(s[0]) - 1].getNormal() + n[stoi(s[2]) - 1]); // need to renormalize
			}
			continue;
		}
	}
	glDownload(v, in);
}

Mesh::Mesh(  vector<Vertex> &vec, vector < unsigned int> &i) : vbo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), ibo([this](GLuint* buf) {deleteBuffer(buf); }, new GLuint), vao(true, true, true)
{
	glDownload( vec, i);
}

void Mesh::initGL( unsigned char flag)
{
	if (flag == 0)
		return;
	if (flag & 2)
	{
		glGenBuffers( 1, vbo.get());
	}
	if (flag & 1)
	{
		glGenBuffers(1, ibo.get());
	}
	vao.createVertexArray();
}

void Mesh::glDownload(vector<Vertex>& v, vector < unsigned int>& i)
{
	initGL(!glIsBuffer( *(vbo.get())) << 1 | !glIsBuffer( *(ibo.get())));

	if (v.size() == 0)
		return;

	vector<float> temp;
	for (unsigned int i = 0; i < v.size(); i++)
	{
		float* d = v[i].getData();
		for (size_t j = 0; j < 8; j++)
		{
			temp.push_back(d[j]);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, *(vbo.get())); //contains Vertices
	glBufferData( GL_ARRAY_BUFFER, temp.size() * 4, temp.data(), GL_STATIC_DRAW);
	temp.clear();

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *(ibo.get()));
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), i.data(), GL_STATIC_DRAW);
	indices = i.size();
}

void Mesh::deleteBuffer(GLuint * buf)
{
	if (glIsBuffer(*buf))
		glDeleteBuffers(1, buf);
}

void Mesh::Draw()
{
	vao.bindVertexArray();
	glBindBuffer(GL_ARRAY_BUFFER, *(vbo.get()));
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *(ibo.get()));

	//vec2 = 8 vec3 = 12 vec4 = 16


	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

	vao.disableVAO();
	//Wireframe Shader
	//glDrawElements( GL_LINE_STRIP, indices, GL_UNSIGNED_INT, 0);  //for debug purposes
}

Mesh::~Mesh(void)
{
}

void Mesh::VertexArrayObject::createVertexArray()
{
	GLuint temp = -1;
	if ( !VAO.get() || !glIsVertexArray(*(VAO.get())))
	{
		glGenVertexArrays(1, &temp);
		VAO.set(new GLuint(temp));
		glBindVertexArray( *(VAO.get()));

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
	glBindVertexArray( *(VAO.get()));

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
	//Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, getsize() /*stride*/, 0 /*offset*/); //Pos
}

void Mesh::VertexArrayObject::disableVec()
{
	glDisableVertexAttribArray(0);
}

void Mesh::VertexArrayObject::enableTex()
{
	//TexCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1 /*Vertex Attribute Layout Location*/, 2 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, getsize() /*stride*/, (void*)12/*offset*/); //Tex
}

void Mesh::VertexArrayObject::disableTex()
{
	glDisableVertexAttribArray(1);
}

void Mesh::VertexArrayObject::enableNor()
{
	//normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, getsize() /*stride*/, (void*)20/*offset*/); //Nor
}

void Mesh::VertexArrayObject::disableNor()
{
	glDisableVertexAttribArray(2);
}

Mesh2D::Mesh2D(vector<Vertex>& vec) : Mesh()
{
	vector<unsigned int> i = vector<unsigned int>({0,1,2,3});
	glDownload(vec, i);
	vao.disableNor();
}

Mesh2D::Mesh2D(vector<Vertex>& vec, vector<unsigned int> &i) : Mesh()
{
	glDownload(vec, i);
	vao.disableNor();
}

void Mesh2D::glDownload(vector<Vertex>& v, vector<unsigned int>& i)
{
	initGL(!glIsBuffer(*(vbo.get())) << 1 | !glIsBuffer(*(ibo.get())));

	if (v.size() == 0)
		return;

	vector<float> temp;
	for (unsigned int i = 0; i < v.size(); i++)
	{
		float* d = v[i].getData();
		for (size_t j = 0; j < 6; j++)
		{
			if (i == 2)//skips Z data
				continue; 
			temp.push_back(d[j]);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, *(vbo.get())); //contains Vertices
	glBufferData(GL_ARRAY_BUFFER, temp.size() * 4, temp.data(), GL_STATIC_DRAW);
	temp.clear();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(ibo.get()));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), i.data(), GL_STATIC_DRAW);
	indices = i.size();
}
