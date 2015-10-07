#include <string>
#include <vector>

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

#pragma once
class Shader
{
public: //Public structures
	enum ShaderType
	{
		VERTEX,
		TESSELATION_CONTROL,
		TESSELATION_EVALUATION,
		GEOMETRY,
		FRAGMENT
	};
	class ShaderCode
	{
	public:
		ShaderType _type; //should be made private
		std::string _path;
		GLuint pos;

		ShaderCode() : _type(TESSELATION_EVALUATION), _path(""), pos(-1) {}
		ShaderCode(ShaderType type, std::string path) : _type(type), _path(path), pos(-1) {}
		ShaderCode(ShaderType type, char* path) : _type(type), _path(path), pos(-1) {}
		~ShaderCode() { if (glIsShader(pos)) glDeleteShader(pos); } //just don't leave stuff behind

		ShaderCode operator=(ShaderCode other) { _type = other._type; _path = other._path; pos = other.pos; return *this; }
		bool operator==(ShaderCode other) { return _type == other._type && _path == other._path && pos == other.pos; }
		bool operator!=(ShaderCode other) { return !(*this == other); }
	};

	class Uniform
	{
	public:
		struct FloatArr //varying amount of floats that are going to be copied
		{
			FloatArr(float* ptr, size_t size) : _ptr(ptr), _size(size){}
			float* _ptr;
			size_t _size;
		};
		typedef std::vector<FloatArr> UniformSet; //all possible Uniform Values depending on the Object
	

		std::string _name;
		UniformSet _data;
			GLuint pos;

		Uniform() : _name("emptyUniform"), _data(), pos(-1) {}
		Uniform(std::string name, UniformSet data) : _name(name), _data(data), pos(-1) {}
		Uniform(std::string name, FloatArr data) : _name(name), _data(), pos(-1) { _data.push_back(data); }

		int addFloatArr(FloatArr data) { _data.push_back(data); return _data.size() - 1; }
		int addFloatArr(Uniform &u) { if (*this == u) { _data.insert(_data.end(), u._data.begin(), u._data.end()); return _data.size() - 1; } else return -1; }

		bool operator==(Uniform other) { return _name == other._name; } //that should suffice
		bool operator!=(Uniform other) { return _name != other._name; } //that should suffice
	};
public:
	//construction / destruction
	Shader();
	Shader(std::string, std::string);
	Shader(ShaderCode[]);
	Shader(std::vector<ShaderCode>);
	~Shader();

	//getter / setter
	void addShader(ShaderCode&);
	int addUniform(Uniform&);

	//functions
	void load();
	void build();
	void bind();
	void setUniforms(unsigned);
	
	//operators
	Shader operator= (Shader);
	Shader operator= (std::vector<ShaderCode>);
	bool operator== (Shader);
	bool operator== (ShaderCode);
	bool operator== (std::vector<ShaderCode>);
private:
	//variables
	//OpenGL
	GLuint program;
	std::vector<ShaderCode> Code; //contains Shader variable
	std::vector<Uniform> Uniforms;

	//functions
	void setupUniform(Uniform*);
	void updateUniform(Uniform*, unsigned);
	void makeShader(ShaderCode*);
	std::string checkProgram();
	GLenum getShaderType(ShaderType);
	std::string getShaderCode(std::string);
};

