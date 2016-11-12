#include <vector>
#include <glm\common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util.h"
#include "Deletors.h"

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

		bool valid() {
			return _vao != nullptr;
		}

		static unsigned char genBitset(int vec, int tex = 0, int norm = 0) { return (vec | (tex << 2) | (norm << 4)); }

	private:
		int getSize() { return (isVec() + isTex() + isNor()) * 4; }

		void enableVec();
		void disableVec();
		void enableTex();
		void disableTex();
		void enableNor();
		void disableNor();

		unique_ptr<GLuint, deleteGLVAO> _vao;
		unsigned char _bitset;
	};
	class Vertex
	{
	public:
		Vertex()
		{
			data[0] = 0; data[1] = 0; data[2] = 0;
			data[3] = 0; data[4] = 0; data[5] = 0;
			data[6] = 0; data[7] = 0; data[8] = 0;
		}
		Vertex(const Vertex& other)
		{
			memcpy(data, other.data, sizeof(data));
		}
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
		void setPos(vec3 pos) { data[0] = pos.x; data[1] = pos.y; data[2] = pos.z; }

		vec3 getPos() { return  vec3(data[0], data[1], data[2]); }
		vec3 getNormal() { return vec3(data[6], data[7], data[8]); }

		bool operator== (const Vertex& other)
		{
			return equal(begin(data), end(data), begin(other.data), end(other.data));
		}

		bool operator!= (const Vertex& other)
		{
			return !equal(begin(data), end(data), begin(other.data), end(other.data));
		}

		float* getData()
		{
			vec3 nor = normalize(vec3(data[6], data[7], data[8]));
			data[6] = nor.x; data[7] = nor.y; data[8] = nor.z;
			return data;
		}

		~Vertex()
		{
			delete[9] data;
		}
	private:
		float data[9];
	};

	struct unnormalizedVertex
	{
		unnormalizedVertex() : _v( vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0))
		{
		}
		unnormalizedVertex(Vertex& v, vector<vec3>& nor) : _v(v), _nor(nor)
		{}

		void operator=(Vertex& v)
		{
			_v = v;
			_nor = vector<vec3>();
		}

		Vertex _v;
		vector<vec3> _nor;
	};

	Mesh ( string&);
	Mesh ( vector<Vertex> &vec, unsigned char bitset); //Rectangle Constructor
	Mesh ( vector<Vertex> &vec, vector < unsigned int> &i, unsigned char bitset);
	explicit Mesh(Mesh&& other);
	Mesh ();
		
	virtual void load(RessourceHandler*);
	virtual void load(ifstream&);
	void* get() { return this; }
	virtual void init();

	virtual void Draw();

	Mesh& operator = (const Mesh& other);

	~Mesh(void);
protected:
	vector<Vertex> getNormalVertices(vector<unnormalizedVertex>);

	void updateVertices(); //updates both Buffers completely
	void updateVertices(unsigned int offset, unsigned int end); //updates Vertices Buffer from off to end
	void updateIndices (unsigned int offset, unsigned int end); //updates Indices Buffer from off to end
	void updateVertices(unsigned int offset, unsigned int end, unsigned int indOffset, unsigned int indEnd); //updates everything

	vector<Vertex> _vertices;
	vector<unsigned int> _indices;
private:
	void initGL(unsigned char);

	string _path;
	
	RessourceHandler::Ressource _loadReq;
	
	VertexArrayObject _vao;
	unique_ptr<GLuint, deleteGLBuffer> _vbo;
	unique_ptr<GLuint, deleteGLBuffer> _ibo;
	unsigned int _VerticesCount;

	void _load(vector<string>);
	void _glDownload( vector<Vertex>&, vector <unsigned int>&);
};
