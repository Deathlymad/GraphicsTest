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
		Uniform(string name, unsigned int size) : _name(name), _data(new float[size]), _size(size), pos(-1), enabled(false) {}
		Uniform(string name, float*& data, unsigned size) : _name(name), _data(new float[size]), _size(0), pos(-1), enabled(false)
		{
			data = _data.get();
			_size = size;
		}

		void create(GLuint* prgm);
		void copy(Uniform& other);
		void write(GLuint* prgm);

		float* getPtr();
		string& getName() { return _name; }

		bool operator==(Uniform& other) { return _name == other._name; }
		bool operator==(string& name) { return _name == name; }
		bool operator!=(Uniform& other) { return _name != other._name; }
		bool operator!=(string& name) { return _name != name; }
		bool operator< (Uniform& other) { return _name.compare(other._name) < 0; }
		bool operator< (string& name) { return _name.compare(name) < 0; }
		bool operator> (Uniform& other) { return _name.compare(other._name) > 0; }
		bool operator> (string& name) { return _name.compare(name) > 0; }

		static int getUniformSize(string& name);

		~Uniform()
		{
			_size = 0;
		}

		bool enabled;
	private:
		string _name;
		Ptr<float> _data;
		unsigned _size;
		GLuint pos;
	};

	class ShaderCode : public RessourceLoader
	{
		string _code;
		CustomPtr<GLuint> _pos;
		ShaderType _type; //should be made private
		string _path;
		Shader* _owner;
	public:
		void clearShader(GLuint* s)
		{
			if (s)
				if (glIsShader(*s))
					glDeleteShader(*s);
		}

		ShaderCode() : _owner(nullptr), _type(TESSELATION_EVALUATION), _path(""), _pos([this](GLuint* s) {clearShader(s); }, new GLuint()) {}
		ShaderCode(Shader* owner, ShaderType type, string path) : _owner(owner), _type(type), _path(path), _pos([this](GLuint* s) {clearShader(s); }, new GLuint())
		{ _pos.setDestructor([this](GLuint* s) {clearShader(s); }); }
		ShaderCode(Shader* owner, ShaderType type, char* path) : _owner(owner), _type(type), _path(path), _pos([this](GLuint* s) {clearShader(s); }, new GLuint()) {}
		
		void load(ifstream&);
		void* get();

		void makeShader();

		GLuint& getPos() { return *_pos.get(); }
		string& getPath() { return _path; }

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
	Shader(string, string);
	Shader(ShaderCode[]);
	Shader(vector<ShaderCode>);
	~Shader();

	//getter / setter
	void addShader(ShaderCode&);
	float* getUniformMemPos(string);

	//functions
	void load(RessourceHandler* loader);

	void build();
	void bind();
	void setUniforms();

	//operators
	Shader& operator= (Shader&);
	Shader& operator= (vector<ShaderCode>&);
	bool operator== (Shader&);
	bool operator== (ShaderCode&);
	bool operator== (vector<ShaderCode>&);
private:
	void addUniform(Uniform& u) {	
		u.enabled = true;
		Uniforms.insert( Uniforms.begin() + findUniform(u.getName(), 0, Uniforms.size()), u);
	}

	//variables
	//OpenGL
	CustomPtr<GLuint> program;
	void deleteProgram(GLuint* prgm)
	{
		if (glIsProgram(*prgm))
			glDeleteProgram(*prgm);
	}

	vector<ShaderCode> Code; //contains Shader variable
	vector<Uniform> Uniforms;
	vector<shared_future<ShaderCode*>*> reqArr;

	//functions
	string checkProgram();
	int findUniform(Uniform&, int, int);
	int findUniform(string&, int, int);
};

