#include <string>
#include <vector>
#include <iostream>

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

#include "Deletors.h"
#include "Util.h"
#include "RessourceHandler.h"

NSP_UTIL
NSP_STD

#pragma once
class Shader
{
	friend class ShaderCode;
public: //Public structures
	enum ShaderType
	{
		ERR,
		VERTEX,
		TESSELATION_CONTROL,
		TESSELATION_EVALUATION,
		GEOMETRY,
		FRAGMENT
	};

	class Uniform
	{
	public:

		Uniform() : _name("emptyUniform"), _data(nullptr), _size(0), pos(-1), enabled(false) {}
		Uniform(string& name, unsigned int size) : _name(name), _data(new float[size]), _size(size), pos(-1), enabled(false) {}
		Uniform(string& name, float*& data, unsigned size) : _name(name), _data(new float[size]), _size(0), pos(-1), enabled(false)
		{
			memcpy(_data.get(), data, size * sizeof(float));
			_size = size;
		}
		Uniform(const Uniform& other) : 
			enabled(other.enabled),
			_name(other._name),
			_size(other._size),
			pos(other.pos),
			_data(new float[other._size])
		{
			memcpy(_data.get(), other._data.get(), other._size * sizeof(float));
		}

		Uniform& operator=(const Uniform& other)
			
		{
			enabled = other.enabled;
			_name = other._name;
			_size = other._size;
			pos = other.pos;

			_data.reset(new float[other._size]);
			memcpy(_data.get(), other._data.get(), other._size * sizeof(float));
			
			return *this;
		}

		void create(GLuint& prgm);
		void copy(Uniform& other);
		void write(GLuint& other);

		void enable() { enabled = true; }
		void disable() { enabled = false; }

		string& getName() { return _name; }

		bool operator==(const Uniform& other) { return _name == other._name; }
		bool operator==(const string& name) { return _name == name; }
		bool operator!=(const Uniform& other) { return _name != other._name; }
		bool operator!=(const string& name) { return _name != name; }
		bool operator< (const Uniform& other) { return _name.compare(other._name) < 0; }
		bool operator< (const string& name) { return _name.compare(name) < 0; }
		bool operator> (const Uniform& other) { return _name.compare(other._name) > 0; }
		bool operator> (const string& name) { return _name.compare(name) > 0; }

		static int getUniformSize(string& name);

		void set(float *val)
		{
			memcpy(_data.get(),val, _size * sizeof(float));
		}

		float* getPtr()
		{
			return _data.get();
		}

		~Uniform()
		{
			_size = 0;
		}

	private:
		bool enabled;
		string _name;
		unique_ptr<float[]> _data;
		unsigned _size;
		GLuint pos;
	};

	class ShaderCode : public RessourceLoader
	{
		string _code;
		unique_ptr<GLuint, deleteGLShader> _pos;
		ShaderType _type;
		string _path;
		Shader* _owner;
		string _loadingErr;
		vector<Uniform> uniforms;
	public:
		//ShaderCode() : _owner(), _type(TESSELATION_EVALUATION), _path(""), _pos( new GLuint(), deleteGLShader()) {}
		ShaderCode(Shader* owner, ShaderType type, string& path) : _owner(owner), _type(type), _path(path), _pos( new GLuint(-1), deleteGLShader()) {}
		ShaderCode(Shader* owner, ShaderType type, char* path) : _owner(owner), _type(type), _path(path), _pos( new GLuint(-1), deleteGLShader()) {}
		ShaderCode(const ShaderCode& other) : 
			_code(other._code),
			_type(other._type),
			_path(other._path),
			_owner(other._owner),
			_loadingErr(other._loadingErr),
			uniforms(other.uniforms), 
			_pos(new GLuint(), deleteGLShader())
		{
		}

		void load(ifstream&);
		void* get();
		
		void makeShader();

		GLuint& getPos() { return *_pos; }
		string& getPath() { return _path; }
		vector<Uniform>& getUniforms() { return uniforms; }

		ShaderCode& operator=(const ShaderCode& other);
		bool operator==(const ShaderCode& other) { return _type == other._type && _path == other._path && _pos == other._pos; }
		bool operator!=(const ShaderCode& other) { return !(*this == other); }
	private:
		GLenum getShaderType(ShaderType);
		vector<vector<string>> structVariables;
		void addUniform(string&);
		vector<string> resolveStructVariable(string&);
	};

public:
	//construction / destruction
	Shader();
	Shader(string&, string&);
	~Shader();

	//getter / setter
	void addShader(ShaderCode&);
	float* getUniformMemPos(string);

	//functions
	void load(RessourceHandler& loader);

	void build();
	void bind();
	void setUniform(const string&, float*);
	void setUniforms();

	//operators
	Shader& operator= (const Shader&);
	Shader& operator= (const vector<ShaderCode>&);
	bool operator== (const Shader&);
	bool operator== (const ShaderCode&);
	bool operator== (const vector<ShaderCode>&);
private:

	//variables
	//OpenGL
	unique_ptr<GLuint, deleteGLProgram> program;

	vector<ShaderCode> Code; //contains Shader variable
	vector<Uniform> Uniforms;
	vector<RessourceHandler::Ressource> reqArr;

	//functions
	string checkProgram();
	int findUniform(const Uniform&, int, int);
	int findUniform(const string&, int, int);
};

