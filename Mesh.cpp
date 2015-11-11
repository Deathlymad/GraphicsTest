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
#endif
#include <glm/gtc/matrix_transform.hpp>

#include "iofunctions.h"

Mesh::Mesh() : vao(-1), ibo(-1)
{
}

Mesh::Mesh ( std::string file)
{
	std::vector<glm::vec3> v;
	std::vector<unsigned int> in;
	std::vector<glm::vec3> n;
	//TODO Load Tex Coordinates
	
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
			v.push_back( glm::vec3( stof(temp[1]), stof(temp[2]), stof(temp[3])));
			continue;
		}
		
		if(temp[0].compare("vt") == 0)
		{
			//t.push_back(glm::vec2(stof(temp[1]),stof(temp[2])));
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
				//if(n.size() > 0)
					//v[std::stoi(s[0]) - 1].setNormal(v[std::stoi(s[0]) - 1].getNormal() + n[std::stoi(s[2]) - 1]);
			}
			continue;
		}
	}
	pts = v;
	ind = in;
	glDownload(v, in);
}

Mesh::Mesh(  std::vector<glm::vec3> &vec, std::vector < unsigned int> &i)
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
		glGenBuffers( 2, &vbo[0]);
	}
	if (flag & 1)
	{
		glGenBuffers(1, &ibo);
	}
}

void Mesh::glDownload(std::vector<glm::vec3>& v, std::vector < unsigned int>& i)
{
	initGL(!glIsBuffer(vbo[0]) << 1 | !glIsBuffer(ibo) | !glIsVertexArray(vao) << 2);

	if (v.size() == 0)
		return;

	std::vector <glm::vec3> norm;
	for (unsigned int h = 0; h < i.size() / 3; h++)
	{
		unsigned int j = h * 3;
		norm.push_back(glm::normalize(glm::cross(v[i[h]] - v[i[h + 1]], v[i[h + 2]] - v[i[h + 1]])));
	}

	std::vector<float> temp;
	temp.clear();
	for (unsigned int i = 0; i < v.size(); i++) // may be obsulete
	{
		temp.push_back(v[i].x);
		temp.push_back(v[i].y);
		temp.push_back(v[i].z);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //contains Vertices
	glBufferData( GL_ARRAY_BUFFER, temp.size() * 4, temp.data(), GL_DYNAMIC_DRAW);
	temp.clear();
	for (unsigned int i = 0; i < norm.size(); i++)
	{
		temp.push_back(norm[i].x);
		temp.push_back(norm[i].y);
		temp.push_back(norm[i].z);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); //contains Normals
	glBufferData(GL_ARRAY_BUFFER, temp.size() * 4, temp.data(), GL_DYNAMIC_DRAW);
	//glVertexAttribPointer( 1 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, 0 /*stride*/, 0 /*offset*/); //Normals
	//glEnableVertexAttribArray( 1);

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), i.data(), GL_DYNAMIC_DRAW);
	indices = i.size();
	vecs = v.size();
}

void Mesh::Draw()
{
	initGL(!glIsBuffer(vbo[0]) << 1 | !glIsBuffer(ibo) | !glIsVertexArray(vao) << 2);
	glBindVertexArray( vao);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo);

	//vec2 = 8 vec3 = 12 vec4 = 16


	glEnableVertexAttribArray(0); //TODO dynamic VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, 12 /*stride*/, 0 /*offset*/); //Pos


	glEnableVertexAttribArray(1); //TODO dynamic VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1 /*Vertex Attribute Layout Location*/, 3 /*amount of Type*/, GL_FLOAT /*Type of Data*/, false /* needs to be normalized*/, 12 /*stride*/, 0 /*offset*/); //Pos

	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	//Wireframe Shader
	//glDrawElements( GL_LINE_STRIP, indices, GL_UNSIGNED_INT, 0);  //for debug purposes

}
Mesh::~Mesh(void)
{
	if (glIsVertexArray(vao))
		glDeleteVertexArrays( 1, &vao);
	if (glIsBuffer(vao))
		glDeleteBuffers( 2, &vbo[0]);
	if (glIsBuffer(vao))
		glDeleteBuffers( 1, &ibo);
}
