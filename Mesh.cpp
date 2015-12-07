#include "Mesh.h"

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
#include <glm/gtc/matrix_transform.hpp>

#include "iofunctions.h"

Mesh::Mesh() : vao(-1), vbo(-1), ibo(-1)
{
}

Mesh::Mesh ( std::string file)
{
	std::vector<Vertex> v;
	std::vector<unsigned int> in;
	std::vector<glm::vec3> n;
	std::vector<glm::vec2> t;
	
	std::vector<std::string> vec;
	std::vector<std::string> temp;
	io::load(vec, file);
		
	for(unsigned int i = 0; i<vec.size(); i++)
	{
		temp.clear();
		if((vec[i].find_first_of('o') != std::string::npos)) continue; //Object type name irrelevant
		if((vec[i].find_first_of('m') != std::string::npos)) continue; //Material Data may be used someday but not now
		if((vec[i].find_first_of('s') != std::string::npos)) continue; //some other data
		if((vec[i].find_first_of('#') != std::string::npos) || vec[i].size() < 3) continue; //Kommentarzeilen werden KOMPLETT ignoriert, auch wenn Kommentar am Ende steht! oder Leerzeilen oder Zeile hat weniger als 3 Zeichen => bestimmt sinnlos
		io::strsep(temp,vec[i]); //vec[i] in teile zerlegen
		if(temp[0].compare("v") == 0)
		{
			v.push_back( Vertex(glm::vec3( stof(temp[1]), stof(temp[2]), stof(temp[3])) , glm::vec2(-1.0, -1.0), glm::vec3(0,1,0))); //writes default Data
			continue;
		}
		
		if(temp[0].compare("vt") == 0)
		{
			t.push_back(glm::vec2(stof(temp[1]),stof(temp[2])));
			continue;
		}
			
		if(temp[0].compare("vn") == 0)
		{
			n.push_back(glm::vec3(stof(temp[1]),stof(temp[2]),stof(temp[3])));
			continue;
		}
		
		if(temp[0].compare("f") == 0) // needs to have a check wether the face ids are seperated by space or by slash
		{
			std::vector<std::string> s;
			
			for (unsigned int i = 1; i < temp.size(); i++)
			{
				io::strsep( s, temp[i], '/');
				in.push_back(std::stoi(s[0]) - 1);
				if (t.size() > 0)
					v[std::stoi(s[0]) - 1].setTexCoord( t[std::stoi(s[1]) - 1]);
				if(n.size() > 0)
					v[std::stoi(s[0]) - 1].setNormal(v[std::stoi(s[0]) - 1].getNormal() + n[std::stoi(s[2]) - 1]); // need to renormalize
			}
			continue;
		}
	}
	ind = in;
	glDownload(v, in);
}

Mesh::Mesh(  std::vector<Vertex> &vec, std::vector < unsigned int> &i)
{
	pts = vec;
	ind = i;
	glDownload( vec, i);
}

void Mesh::initGL( unsigned char flag)
{
	if (flag == 0)
		return;
	if (flag & 4)
	{
		glGenVertexArrays( 1, &vao);
	}
		
	if (flag & 2)
	{
		glGenBuffers( 1, &vbo);
	}
	if (flag & 1)
	{
		glGenBuffers(1, &ibo);
	}
}

void Mesh::glDownload(std::vector<Vertex>& v, std::vector < unsigned int>& i)
{
	initGL(!glIsBuffer(vbo) << 1 | !glIsBuffer(ibo) | !glIsVertexArray(vao) << 2);

	if (v.size() == 0)
		return;

	std::vector<float> temp;
	for (unsigned int i = 0; i < v.size(); i++)
	{
		float* d = v[i].getData();
		for (size_t j = 0; j < 8; j++)
		{
			temp.push_back(d[j]);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //contains Vertices
	glBufferData( GL_ARRAY_BUFFER, temp.size() * 4, temp.data(), GL_DYNAMIC_DRAW);
	temp.clear();

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), i.data(), GL_DYNAMIC_DRAW);
	indices = i.size();
	DataSize = v.size() * 12;
}

void Mesh::Draw()
{
	initGL(!glIsBuffer(vbo) << 1 | !glIsBuffer(ibo) | !glIsVertexArray(vao) << 2);
	glBindVertexArray( vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo);

	//vec2 = 8 vec3 = 12 vec4 = 16

	//Vertices
	glEnableVertexAttribArray(0); //TODO dynamic VAO
	glVertexAttribPointer(0 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, 32 /*stride*/, 0 /*offset*/); //Pos
	//TexCoords
	glEnableVertexAttribArray(1); //TODO dynamic VAO
	glVertexAttribPointer(1 /*Vertex Attribute Layout Location*/, 2 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, 32 /*stride*/, (void*)12/*offset*/); //Tex
	//normals
	glEnableVertexAttribArray(2); //TODO dynamic VAO
	glVertexAttribPointer(2 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, 32 /*stride*/, (void*)20/*offset*/); //Nor

	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);
	//Wireframe Shader
	//glDrawElements( GL_LINE_STRIP, indices, GL_UNSIGNED_INT, 0);  //for debug purposes

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 2 );
	

}
Mesh::~Mesh(void)
{
	if (glIsVertexArray(vao))
		glDeleteVertexArrays( 1, &vao);
	if (glIsBuffer(vao))
		glDeleteBuffers( 1, &vbo);
	if (glIsBuffer(vao))
		glDeleteBuffers( 1, &ibo);
}
