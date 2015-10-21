#include <fstream>
#include <algorithm>
#include <iostream>

#include "Shader.h"



Shader::Shader()
{
	program = -1;
	Code.clear();
}

Shader::Shader(std::string vertexPath, std::string fragPath)
{
	std::vector<ShaderCode> temp;
	temp.push_back(ShaderCode(VERTEX, vertexPath));
	temp.push_back(ShaderCode(FRAGMENT, fragPath));
	program = -1;
	Code = temp;

	load();
	build();
}

Shader::Shader(ShaderCode ShaderArr[])
{
	std::vector<ShaderCode> temp;
	temp.insert(temp.begin(), sizeof(ShaderArr) / sizeof(ShaderCode), ShaderArr[0]);//copies Array in vector
	program = -1;
	Code = temp;

	load();
	build();
}

Shader::Shader(std::vector<ShaderCode> Shaders)
{
	program = -1;
	Code = Shaders;

	load();
	build();
}

Shader::~Shader()
{
	if (glIsProgram(program))
		glDeleteProgram(program);
	program = -1;
	Code.~vector();
}



void Shader::addShader(ShaderCode &code)
{
	Code.push_back(code);
}
int Shader::addUniform(Uniform &u)
{
	Uniforms.push_back(u);
	return Uniforms.size() - 1;
}



void Shader::load()
{
	for (size_t i = 0; i < Code.size(); i++)
		makeShader(&Code[i]);
}
void Shader::build()
{
	if (!glIsProgram(program))
	{
		program = glCreateProgram(); //no Program existent creating
	} //if there is a Program created with the right type then it is going to be rewritten but not recreated should reduce memory

	int attached = 0;
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &attached);
	if (attached) //in case old Shaders are attached
	{
		std::vector<GLuint> shaders = std::vector<GLuint>();
		for (char i = 0; i < 16; i++) //write invaid numbers to seperate them
			shaders.push_back(-1);
		glGetAttachedShaders(program, 16, NULL, &shaders[0]); //get all Shaders attached to Program
		for (GLuint shader : shaders)
			if (shader != -1) //check if it is a valid shader
				glDetachShader(program, shader); //detaches Shader, DOES NO DELETE
		attached = 0; //resets counter
	}

	for (ShaderCode shader : Code)
	{
		//should check Shader values
		glAttachShader(program, shader.pos);
		int ShaderCount = 0;
		glGetProgramiv(program, GL_ATTACHED_SHADERS, &ShaderCount); //might turn out t be slow since it querries operations and wait for them
		if (ShaderCount > attached) //used for Error catching
			attached++;
	}

	if (attached > 0)
		glLinkProgram(program); //FS Shader not compiled...
	else
		std::cout << "OpenGL Program " << "NO SHADERS ATTACHED." << std::endl;

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (!isLinked)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength > 1)
		{
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog = std::vector<GLchar>(maxLength);
			glGetProgramInfoLog(program, maxLength, NULL, &infoLog[0]);
			std::cout << "OpenGL Program " << infoLog.data() << std::endl;
		}
	}

	//Uniform Phase
	for (size_t i = 0; i < Uniforms.size(); i++)
		setupUniform(&Uniforms[i]);
}
void Shader::bind()
{
	std::string err = checkProgram();

	setUniforms();

	if (err == "")
		glUseProgram(program);
	else
		std::cout << "OpenGL Shader " << err << std::endl;
}
void Shader::setUniforms()
{
	for (size_t i = 0; i < Uniforms.size(); i++)
		updateUniform(&Uniforms[i]);
}



Shader Shader::operator=(Shader other)
{
	Code = other.Code;
	load(); //handles the Shader changes
	build();//handles the Shader changes on Program Level
	return *this;
}

Shader Shader::operator=(std::vector<ShaderCode> Shaders)
{
	Code = Shaders;
	load();
	build();
	return *this;
}

bool Shader::operator==(Shader other)
{
	if (Code.size() != other.Code.size())
		return false;
	for (size_t i = 0; i < Code.size(); i++)
		if (Code[i] != other.Code[i])
			return false;
	return program == other.program;
}

bool Shader::operator==(ShaderCode contained)
{
	
	ShaderCode res = *std::find(Code.begin(), Code.end(), contained)._Ptr;
	return res == contained;
	return true;
}

bool Shader::operator==(std::vector<ShaderCode> Shaders)
{
	bool res = true;
	for (ShaderCode s : Shaders)
	{
		res = res && (*this == s);
	}
	return res;
}



void Shader::setupUniform(Uniform * uniform)
{
	uniform->pos = glGetUniformLocation(program, uniform->_name.c_str()); //Error Handling?
}

void Shader::updateUniform(Uniform * uniform)
{
	float* tempData = uniform->_data;
	glUniform1fv(uniform->pos, sizeof(tempData), tempData); //error Handling
}

void Shader::makeShader(ShaderCode* code)
{
	if (!glIsShader(code->pos))
	{
		code->pos = glCreateShader(getShaderType(code->_type)); //no Shader existent; creating
	}
	else
	{
		int type = 0;
		glGetShaderiv(code->pos, GL_SHADER_TYPE, &type); //if there is already a Shader, check if it has the right type
		if (type != code->_type)
		{
			glDeleteShader(code->pos); //clear Shader if it is of the wrong Type (issues with programs created from it?!)
			code->pos = glCreateShader(code->_type); //recreate Shader of right Type
		}
	}//if there is a Shader created with the right type then it is going to be rewritten but not recreated should reduce memory
	
	std::string s = getShaderCode(code->_path);
	if (s.size() > 1)
	{
		const char *c_str = s.c_str();
		glShaderSource(code->pos, 1, &c_str, NULL);
	}
	else
		std::cout << "OpenGL Shader " << "Empty Source." << std::endl;

	int compiled = 0;
	glGetShaderiv(code->pos, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
		glCompileShader(code->pos);

	glGetShaderiv(code->pos, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{ //not compiling, no error Log....
		GLint maxLength = 0;
		glGetShaderiv(code->pos, GL_INFO_LOG_LENGTH, &maxLength);
		if (maxLength > 1)
		{
			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog = std::vector<GLchar>(maxLength);
			glGetShaderInfoLog(code->pos, maxLength, NULL, &infoLog[0]);
			std::cout << "OpenGL Shader " << infoLog.data() << std::endl;
		}
	}
}

std::string Shader::checkProgram()
{
	if (program == -1)
		return "NO PROGRAM CREATED \n";
	std::string errText = "";
	int res = 0;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &res);
	if (!res)
	{
		errText += "NOT LINKED \n";
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		if (maxLength > 1)
		{
			std::string infoLog;
			infoLog.resize(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			errText += infoLog;
		}
	}
	return errText;
}

GLenum Shader::getShaderType(ShaderType shaderType)
{
	switch (shaderType)
	{
	case VERTEX:
		return GL_VERTEX_SHADER;
	case TESSELATION_CONTROL:
		return GL_TESS_CONTROL_SHADER;
	case TESSELATION_EVALUATION:
		return GL_TESS_EVALUATION_SHADER;
	case GEOMETRY:
		return GL_GEOMETRY_SHADER;
	case FRAGMENT:
		return GL_FRAGMENT_SHADER;
	default:
		return -1;
	}
}

std::string Shader::getShaderCode(std::string File)
{
	std::string ShaderCode;
	std::ifstream ShaderStream("assets/shaders/" + File);
	if (ShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(ShaderStream, Line))
			ShaderCode += Line + "\n";
		ShaderStream.close();
	}
	else
		return "";
	return ShaderCode;
}
