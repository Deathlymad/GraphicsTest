#include <vector>
#include <glm\common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util.h"

NSP_UTIL
NSP_IO
NSP_STD
NSP_GLM

typedef unsigned int GLuint;

#pragma once
class Mesh
{
public:
	class VertexArrayObject
	{
	public:
		VertexArrayObject() : VAO( nullptr) {}
		VertexArrayObject( int vec, int tex, int norm) : VAO( nullptr), bitset( vec | (tex << 1) | (norm << 2) ) {}
		VertexArrayObject(unsigned char bitset) : VAO( nullptr), bitset(bitset) {}

		void setBitset(int vec, int tex, int norm) { bitset = (vec | (tex << 1) | (norm << 2)); }

		void createVertexArray();
		void bindVertexArray();
		void disableVAO();

		bool isVec() { return (bitset & 1) != 0; }
		bool isTex() { return (bitset & 2) != 0; }
		bool isNor() { return (bitset & 4) != 0; }

		~VertexArrayObject();
	private:
		int getsize() { return (isVec() ? 12 : 0) + (isTex() ? 8 : 0) + (isNor() ? 12 : 0); }

		void enableVec();
		void disableVec();
		void enableTex();
		void disableTex();
		void enableNor();
		void disableNor();

		Ptr<unsigned int> VAO;
		unsigned char bitset;
	};
	class Vertex
	{
	public:
		Vertex(vec3 pos, vec2 tex, vec3 nor)
		{
			data[0] = pos.x; data[1] = pos.y; data[2] = pos.z;
			data[3] = tex.x; data[4] = tex.y;
			data[5] = nor.x; data[6] = nor.y; data[7] = nor.z;
		}

		void setTexCoord( vec2 tex) { data[3] = tex.x; data[4] = tex.y; }

		void setNormal( vec3 nor) { data[5] = nor.x; data[6] = nor.y; data[7] = nor.z; }

		vec3 getNormal() { return vec3(data[5], data[6], data[7]); }

		float* getData()
		{
			vec3 nor = normalize(vec3(data[5], data[6], data[7]));
			data[5] = nor.x; data[6] = nor.y; data[7] = nor.z;
			return data;
		}
	private:
		float data[8];
	};

	Mesh ( string);
	Mesh(  vector<Vertex> &vec, vector < unsigned int> &i);
	Mesh();
		
	void Draw();

	~Mesh(void);
protected:
	void initGL(unsigned char);
	VertexArrayObject vao;
	CustomPtr<GLuint> vbo;
	CustomPtr<GLuint> ibo;
	void deleteBuffer(GLuint* buf);
	unsigned int indices;
private:
	virtual void glDownload(  vector<Vertex>&, vector <unsigned int>&);
};

class Mesh2D : public Mesh
{
public:
	Mesh2D() : Mesh() {}
	Mesh2D(vector<Vertex> &vec);
	Mesh2D(vector<Vertex> &vec, vector<unsigned int>&);
private:
	void glDownload(vector<Vertex>&, vector<unsigned int>&);
};