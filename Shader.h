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

NSP_UTIL

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
		CustomPtr<GLuint> pos;
		void clearShader(GLuint* s)
		{
			if (pos.get())
				if (glIsShader(*(pos.get())))
					glDeleteShader(*(pos.get()));
		}

		ShaderCode() : _type(TESSELATION_EVALUATION), _path(""), pos([this](GLuint* s) {clearShader(s); }, new GLuint()) {}
		ShaderCode(ShaderType type, std::string path) : _type(type), _path(path), pos([this](GLuint* s) {}, new GLuint())
		{ pos.setDestructor([this](GLuint* s) {clearShader(s); }); }
		ShaderCode(ShaderType type, char* path) : _type(type), _path(path), pos([this](GLuint* s) {clearShader(s); }, new GLuint()) {}

		ShaderCode operator=(ShaderCode other) { _type = other._type; _path = other._path; pos = other.pos; return *this; }
		bool operator==(ShaderCode other) { return _type == other._type && _path == other._path && pos.get() == other.pos.get(); }
		bool operator!=(ShaderCode other) { return !(*this == other); }
	};

	class Uniform
	{
	public:

		Uniform() : _name("emptyUniform"), _data(nullptr), _size(0), pos(-1) {}
		Uniform(std::string name, float*& data, unsigned size) : _name(name), _data(new float[size]), _size(0), pos(-1)
		{
			data = _data.get();
			_size = size;
		}

		void create(GLuint* prgm);

		void write(GLuint* prgm);

		bool operator==(Uniform other) { return _name == other._name; } //that should suffice
		bool operator!=(Uniform other) { return _name != other._name; } //that should suffice
		~Uniform()
		{
			_size = 0;
		}

	private:
		std::string _name;
		Ptr<float> _data;
		unsigned _size; //just to make sure sizeof seems derpy...
		GLuint pos;
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
	Shader& operator= (Shader&);
	Shader& operator= (std::vector<ShaderCode>);
	bool operator== (Shader&);
	bool operator== (ShaderCode&);
	bool operator== (std::vector<ShaderCode>);
private:
	//variables
	//OpenGL
	CustomPtr<GLuint> program;
	void deleteProgram(GLuint* prgm)
	{
		if (glIsProgram(*prgm))
			glDeleteProgram(*prgm);
	}

	std::vector<ShaderCode> Code; //contains Shader variable
	std::vector<Uniform> Uniforms;

	//functions
	void makeShader(ShaderCode*);
	std::string checkProgram();
	GLenum getShaderType(ShaderType);
	std::string getShaderCode(std::string);

	//common Shader Files, like libraries :D
	const static ShaderCode Light;   //Contains the basic Light function
	const static ShaderCode Color;   //Defines the Color Funtion to retreive fragment color from the texture, also defines the Uniforms
	const static ShaderCode Vertex;
};

