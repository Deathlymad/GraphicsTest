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

//#define ARRAY_SIZE (sizeof(myArray)/sizeof(*myArray))

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
		std::string _name;
		float* _data; //does not allocate in itself!!!
		unsigned _size; //just to make sure sizeof seems derpy...
		GLuint pos;

		Uniform() : _name("emptyUniform"), _data(nullptr), _size(0), pos(-1) {}
		Uniform(std::string name, float* data, unsigned size) : _name(name), _data(nullptr), _size(0), pos(-1)
		{
			if (data)
			{
				_data = data;
				_size = size;
			}
		}

		void setFloatArr(float* data, unsigned size)
		{
			_data = data;
			_size = size;
		}

		bool operator==(Uniform other) { return _name == other._name; } //that should suffice
		bool operator!=(Uniform other) { return _name != other._name; } //that should suffice
		~Uniform()
		{
			_data = nullptr;
			_size = 0;
		}
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
	void setUniforms();
	
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
	void updateUniform(Uniform*);
	void makeShader(ShaderCode*);
	std::string checkProgram();
	GLenum getShaderType(ShaderType);
	std::string getShaderCode(std::string);
};

