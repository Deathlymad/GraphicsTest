#include <vector>
#include <glm\common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util.h"

NSP_UTIL

typedef unsigned int GLuint;

#pragma once
class Mesh
{
	class VertexArrayObject
	{
	public:
		VertexArrayObject() : VAO( nullptr) {}
		VertexArrayObject( int vec, int tex, int norm) : VAO( nullptr), bitset( vec | (tex << 1) | (norm << 2) ) {}
		VertexArrayObject(unsigned char bitset) : VAO( nullptr), bitset(bitset) {}

		void createVertexArray();
		void bindVertexArray();
		void disableVAO();

		~VertexArrayObject();
	private:
		bool isVec() { return (bitset & 1) != 0; }
		bool isTex() { return (bitset & 2) != 0; }
		bool isNor() { return (bitset & 4) != 0; }

		void enableVec();
		void disableVec();
		void enableTex();
		void disableTex();
		void enableNor();
		void disableNor();

		int getsize() { return (isVec() ? 12 : 0) + (isTex() ? 8 : 0) + (isNor() ? 12 : 0); }

		Ptr<unsigned int> VAO;
		unsigned char bitset;
	};
	class Vertex
	{
	public:
		Vertex(glm::vec3 pos, glm::vec2 tex, glm::vec3 nor)
		{
			data[0] = pos.x; data[1] = pos.y; data[2] = pos.z;
			data[3] = tex.x; data[4] = tex.y;
			data[5] = nor.x; data[6] = nor.y; data[7] = nor.z;
		}

		void setTexCoord( glm::vec2 tex) { data[3] = tex.x; data[4] = tex.y; }

		void setNormal( glm::vec3 nor) { data[5] = nor.x; data[6] = nor.y; data[7] = nor.z; }

		glm::vec3 getNormal() { return glm::vec3(data[5], data[6], data[7]); }

		float* getData()
		{
			glm::vec3 nor = glm::normalize(glm::vec3(data[5], data[6], data[7]));
			data[5] = nor.x; data[6] = nor.y; data[7] = nor.z;
			return data;
		}
	private:
		float data[8];
	};
public:
	Mesh ( std::string);
	Mesh(  std::vector<Vertex> &vec, std::vector < unsigned int> &i);
	Mesh();
		
	void Draw();

	~Mesh(void);
private:
	void initGL(unsigned char);

	void glDownload(  std::vector<Vertex>&, std::vector <unsigned int>&);
	
	VertexArrayObject vao;
	CustomPtr<GLuint> vbo;
	CustomPtr<GLuint> ibo;
	void deleteBuffer(GLuint* buf);

	unsigned int DataSize;
	unsigned int indices;
	//needed for Physics later
	std::vector<Vertex> pts;
	std::vector<unsigned int> ind;
};