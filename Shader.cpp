#include <fstream>
#include <algorithm>
#include <iostream>

#include "Shader.h"

const Shader::ShaderCode Shader::Vertex  = Shader::ShaderCode(VERTEX, "vertex.glsl");
const Shader::ShaderCode Shader::Light   = Shader::ShaderCode(FRAGMENT, "light.glsl");
const Shader::ShaderCode Shader::Color   = Shader::ShaderCode(FRAGMENT, "color.glsl");

Shader::Shader() : program([this](GLuint* p) {deleteProgram(p); })
{
	Code.clear();
}

Shader::Shader(string vertexPath, string fragPath) : program([this](GLuint* p) {deleteProgram(p); })
{
	Code.push_back(Vertex);
	Code.push_back(Color);
	Code.push_back(Light);
	Code.push_back(ShaderCode(VERTEX, vertexPath));
	Code.push_back(ShaderCode(FRAGMENT, fragPath));

	load();
	build();
}

Shader::Shader(ShaderCode ShaderArr[]) : program([this](GLuint* p) {deleteProgram(p); })
{
	Code.push_back(Vertex);
	Code.push_back(Color);
	Code.push_back(Light);
	Code.insert(Code.begin() + 3, sizeof(ShaderArr) / sizeof(ShaderCode), ShaderArr[0]);//copies Array in vector

	load();
	build();
}

Shader::Shader(vector<ShaderCode> Shaders) : program([this](GLuint* p) {deleteProgram(p); })
{
	Code = Shaders;

	Code.push_back(Vertex);
	Code.push_back(Color);
	Code.push_back(Light);

	load();
	build();
}

Shader::~Shader(){}



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
	if (!program.get() || !glIsProgram(*(program.get())))
		program.set( new GLuint( glCreateProgram())); //no Program existent creating

	int attached = 0;
	glGetProgramiv(*(program.get()), GL_ATTACHED_SHADERS, &attached);
	if (attached) //in case old Shaders are attached
	{
		vector<GLuint> shaders = vector<GLuint>();
		for (char i = 0; i < 16; i++) //write invaid numbers to seperate them
			shaders.push_back(-1);
		glGetAttachedShaders(*(program.get()), 16, NULL, &shaders[0]); //get all Shaders attached to Program
		for (GLuint shader : shaders)
			if (shader != -1) //check if it is a valid shader
				glDetachShader(*(program.get()), shader); //detaches Shader, DOES NO DELETE
		attached = 0; //resets counter
	} else

	for (ShaderCode shader : Code)
	{
		//should check Shader values
		glAttachShader(*(program.get()), *(shader.pos.get()));
		int ShaderCount = 0;
		glGetProgramiv( *(program.get()), GL_ATTACHED_SHADERS, &ShaderCount); //might turn out t be slow since it querries operations and wait for them
		if (ShaderCount > attached) //used for Error catching
			attached++;
	}

	if (attached > 0)
		glLinkProgram(*(program.get()));
	else
		cout << "OpenGL Program " << "NO SHADERS ATTACHED." << endl;

	GLint isLinked = 0;
	glGetProgramiv(*(program.get()), GL_LINK_STATUS, (int *)&isLinked);
	if (!isLinked)
	{
		GLint maxLength = 0;
		glGetProgramiv(*(program.get()), GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength > 1)
		{
			//The maxLength includes the NULL character
			vector<GLchar> infoLog = vector<GLchar>(maxLength);
			glGetProgramInfoLog(*(program.get()), maxLength, NULL, &infoLog[0]);
			cout << "OpenGL Program " << infoLog.data() << endl;
		}
	}

	//Uniform Phase
	setUniforms();
}
void Shader::bind()
{
	string err = checkProgram();

	setUniforms();

	if (err == "")
		glUseProgram(*(program.get()));
	else
		cout << "OpenGL Shader " << err << endl;
}
void Shader::setUniforms()
{
	for (size_t i = 0; i < Uniforms.size(); i++)
		Uniforms[i].write(program.get());
}



Shader& Shader::operator=(Shader& other)
{
	Code = other.Code;
	load(); //handles the Shader changes
	build();//handles the Shader changes on Program Level
	return *this;
}

Shader& Shader::operator=(vector<ShaderCode> Shaders)
{
	Code = Shaders;
	load();
	build();
	return *this;
}

bool Shader::operator==(Shader& other)
{
	if (Code.size() != other.Code.size())
		return false;
	for (size_t i = 0; i < Code.size(); i++)
		if (Code[i] != other.Code[i])
			return false;
	return program.get() == other.program.get();
}

bool Shader::operator==(ShaderCode& contained)
{
	
	ShaderCode res = *find(Code.begin(), Code.end(), contained)._Ptr;
	return res == contained;
	return true;
}

bool Shader::operator==(vector<ShaderCode> Shaders)
{
	bool res = true;
	for (ShaderCode s : Shaders)
	{
		res = res && (*this == s);
	}
	return res;
}

void Shader::makeShader(ShaderCode* code)
{
	if (!glIsShader(*(code->pos.get())))
	{
		code->pos.set( new GLuint(glCreateShader(getShaderType(code->_type)))); //no Shader existent; creating
	}
	else
	{
		int type = 0;
		glGetShaderiv(*(code->pos.get()), GL_SHADER_TYPE, &type); //if there is already a Shader, check if it has the right type
		if (type != getShaderType(code->_type))
		{
			glDeleteShader(*(code->pos.get())); //clear Shader if it is of the wrong Type (issues with Programs created from it?!)
			code->pos.set( new GLuint(glCreateShader(getShaderType(code->_type)))); //recreate Shader of right Type
		}
	}//if there is a Shader created with the right type then it is going to be rewritten but not recreated should reduce memory
	
	string s = getShaderCode(code->_path);
	if (s.size() > 1)
	{
		const char *c_str = s.c_str();
		glShaderSource(*(code->pos.get()), 1, &c_str, NULL);
	}
	else
		cout << "OpenGL Shader " << "Empty Source." << endl;

	int compiled = 0;
	glGetShaderiv(*(code->pos.get()), GL_COMPILE_STATUS, &compiled);
	if (!compiled)
		glCompileShader(*(code->pos.get()));

	glGetShaderiv(*(code->pos.get()), GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{ //not compiling
		GLint maxLength = 0;
		glGetShaderiv(*(code->pos.get()), GL_INFO_LOG_LENGTH, &maxLength);
		if (maxLength > 1)
		{
			//The maxLength includes the NULL character
			vector<GLchar> infoLog = vector<GLchar>(maxLength);
			glGetShaderInfoLog(*(code->pos.get()), maxLength, NULL, &infoLog[0]);
			cout << "OpenGL Shader " << infoLog.data() << endl;
		}
	}
}

string Shader::checkProgram()
{
	if ( *(program.get()) == -1)
		return "NO PROGRAM CREATED \n";
	string errText = "";
	int res = 0;
	glValidateProgram(*(program.get()));
	glGetProgramiv(*(program.get()), GL_VALIDATE_STATUS, &res);
	if (!res)
	{
		errText += "NOT LINKED \n";
		int maxLength = 0;
		glGetProgramiv(*(program.get()), GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		if (maxLength > 1)
		{
			string infoLog;
			infoLog.resize(maxLength);
			glGetProgramInfoLog(*(program.get()), maxLength, &maxLength, &infoLog[0]);
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

string Shader::getShaderCode(string File)
{
	string ShaderCode;
	ifstream ShaderStream("assets/shaders/" + File);
	if (ShaderStream.is_open())
	{
		string Line = "";
		while (getline(ShaderStream, Line))
			ShaderCode += Line + "\n";
		ShaderStream.close();
	}
	else
		return "";
	return ShaderCode;
}

void Shader::Uniform::create(GLuint* prgm)
{
	pos = glGetUniformLocation(*prgm, _name.c_str());
}

void Shader::Uniform::write(GLuint* prgm)
{
	if ( pos == -1)
		create(prgm);

	switch (_size)
	{
	case 1:
		glUniform1fv(pos, 1, _data.get()); //error Handling
		break;
	case 2:
		glUniform2fv(pos, 1, _data.get()); //error Handling
		break;
	case 3:
		glUniform3fv(pos, 1, _data.get()); //error Handling
		break;
	case 4:
		glUniform4fv(pos, 1, _data.get()); //error Handling
		break;
	case 9:
		glUniformMatrix3fv(pos, 1, false, _data.get()); //error Handling
		break;
	case 16:
		glUniformMatrix4fv(pos, 1, false, _data.get()); //error Handling
		break;
	default:
		glUniform1fv(pos, _size, _data.get()); //error Handling
		break;
	}
}
