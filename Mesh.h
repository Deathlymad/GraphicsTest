#include <vector>
#include <glm\common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util.h"

#include "RessourceHandler.h"

NSP_UTIL
NSP_IO
NSP_STD
NSP_GLM

typedef unsigned int GLuint;

#pragma once
class Mesh : RessourceLoader
{
public:
	class VertexArrayObject //bitset works as long as we stay in 3D space
	{
	public:
		VertexArrayObject() : _vao( nullptr) {}
		VertexArrayObject( int vec, int tex, int norm) : _vao( nullptr), _bitset( vec | (tex << 2) | (norm << 4) ) {}
		VertexArrayObject(unsigned char _bitset) : _vao( nullptr), _bitset(_bitset) {}

		void setBitset(unsigned char set) { _bitset = set; }
		void setBitset(int vec, int tex = 0, int norm = 0) { _bitset = (vec | (tex << 2) | (norm << 4)); }

		void createVertexArray();
		void bindVertexArray();
		void disableVAO();

		char isVec() { return (_bitset & 3); } //first two bits
		char isTex() { return (_bitset & 12) >> 2; }
		char isNor() { return (_bitset & 48) >> 4; }

		~VertexArrayObject();
		static unsigned char genBitset(int vec, int tex = 0, int norm = 0) { return (vec | (tex << 2) | (norm << 4)); }

	private:
		int getSize() { return (isVec() + isTex() + isNor()) * 4; }

		void enableVec();
		void disableVec();
		void enableTex();
		void disableTex();
		void enableNor();
		void disableNor();

		Ptr<GLuint> _vao;
		unsigned char _bitset;
	};
	class Vertex
	{
	public:
		Vertex(vec2 pos, vec2 tex, vec3 nor = vec3(0,0,0))
		{
			data[0] = pos.x; data[1] = pos.y; data[2] =     0;
			data[3] = tex.x; data[4] = tex.y; data[5] =     0;
			data[6] = nor.x; data[7] = nor.y; data[8] = nor.z;
		}

		Vertex(vec3 pos, vec2 tex, vec3 nor = vec3(0, 0, 0))
		{
			data[0] = pos.x; data[1] = pos.y; data[2] = pos.z;
			data[3] = tex.x; data[4] = tex.y; data[5] =     0;
			data[6] = nor.x; data[7] = nor.y; data[8] = nor.z;
		}
		Vertex(vec3 pos, vec3 tex, vec3 nor = vec3(0, 0, 0))
		{
			data[0] = pos.x; data[1] = pos.y; data[2] = pos.z;
			data[3] = tex.x; data[4] = tex.y; data[5] = tex.z; //needed for 3D textures
			data[6] = nor.x; data[7] = nor.y; data[8] = nor.z;
		}

		void setTexCoord( float tex) { data[3] = tex  ; data[4] = -1   ; data[5] = -1  ; }
		void setTexCoord( vec2  tex) { data[3] = tex.x; data[4] = tex.y; data[5] = 0   ; }
		void setTexCoord( vec3  tex) { data[3] = tex.x; data[4] = tex.y; data[5] = tex.z;}

		void setNormal( vec3 nor) { data[6] = nor.x; data[7] = nor.y; data[8] = nor.z; }

		vec3 getNormal() { return vec3(data[6], data[7], data[8]); }

		float* getData()
		{
			vec3 nor = normalize(vec3(data[6], data[7], data[8]));
			data[6] = nor.x; data[7] = nor.y; data[8] = nor.z;
			return data;
		}
	private:
		float data[9];
	};

	struct unnormalizedVertex
	{
		unnormalizedVertex(Vertex v, vector<vec3> nor) : _v(v)
		{
			_nor = nor;
		}
		Vertex _v;
		vector<vec3> _nor;
	};

	Mesh ( string);
	Mesh ( vector<Vertex> &vec, unsigned char bitset); //Square Constructor
	Mesh ( vector<Vertex> &vec, vector < unsigned int> &i, unsigned char bitset);
	Mesh ();
	
	void load(RessourceHandler*);
	void load(ifstream&);
	void* get() { return this; }
	void init();

	void Draw();

	~Mesh(void);
protected:
	void initGL(unsigned char);
	VertexArrayObject _vao;
	CustomPtr<GLuint> _vbo;
	CustomPtr<GLuint> _ibo;
	void deleteBuffer(GLuint* buf);
	unsigned int _VerticesCount;
private:
	string _path;
	
	shared_future<Mesh*>* _laodReq;
	
	vector<unnormalizedVertex> _vertices;
	vector<unsigned int> _indices;

	void _load(vector<string>);
	void _glDownload( vector<Vertex>&, vector <unsigned int>&);
	vector<Vertex> getNormalVertices(vector<unnormalizedVertex>);
};
