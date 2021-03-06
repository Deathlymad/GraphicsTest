#include <fstream>
#include <algorithm>
#include <iostream>
#include <regex>

#include "Shader.h"

#include "Log.h"

Shader::Shader() : Code(), program(new GLuint(), deleteGLProgram())
{
	Code.clear();
}

Shader::Shader(string& vertexPath, string& fragPath) : Code(), program(new GLuint(-1), deleteGLProgram())
{
	Code.push_back(move(ShaderCode( this, VERTEX, vertexPath)));
	Code.push_back(move(ShaderCode( this, FRAGMENT, fragPath)));
}

Shader::~Shader(){}

void Shader::addShader(ShaderCode &code)
{
	Code.push_back(code);
}

float* Shader::getUniformMemPos(string name)
{
	unsigned int pos = findUniform(name, 0, Uniforms.size());

	if (pos >= Uniforms.size())
		return nullptr;

	if (Uniforms[pos] == name)
	{
		return Uniforms[pos].getPtr();
	}
	return nullptr;
}

void Shader::load(RessourceHandler& loader)
{
	for (ShaderCode& code : Code)
		reqArr.push_back(loader.getRessource(code.getPath(), &code));
}

void Shader::build()
{
	if (Code.size() == reqArr.size())
	{
		for (size_t i = 0; i < Code.size(); i++)
		{
			Code[i] = *((ShaderCode*)reqArr[i].get());
			Code[i].makeShader();
			vector<Uniform>& t = Code[i].getUniforms();
			for (Uniform& temp : t)
				Uniforms.insert(Uniforms.begin() + findUniform(temp.getName(), 0, Uniforms.size()), temp);
		}
	}

	if (!program.get() || !glIsProgram(*program))
		*program = glCreateProgram(); //no Program existent creating

	int attached = 0;
	glGetProgramiv(*program, GL_ATTACHED_SHADERS, &attached);
	if (attached) //in case old Shaders are attached
	{
		vector<GLuint> shaders = vector<GLuint>();
		for (char i = 0; i < 16; i++) //write invaid numbers to seperate them
			shaders.push_back(-1);
		glGetAttachedShaders(*program, 16, NULL, &shaders[0]); //get all Shaders attached to Program
		for (GLuint shader : shaders)
			if (shader != -1) //check if it is a valid shader
				glDetachShader(*program, shader); //detaches Shader, DOES NO DELETE
		attached = 0; //resets counter
	}

	for (ShaderCode& shader : Code)
	{
		//should check Shader values
		glAttachShader(*program, shader.getPos());
		int ShaderCount = 0;
		glGetProgramiv( *program, GL_ATTACHED_SHADERS, &ShaderCount); //might turn out t be slow since it querries operations and wait for them
		if (ShaderCount > attached) //used for Error catching
			attached++;
	}

	if (attached > 0)
		glLinkProgram(*program);
	else
		LOG << "OpenGL Program " << "NO SHADERS ATTACHED." << "\n";

	int isLinked = 0;
	glGetProgramiv(*program, GL_LINK_STATUS, &isLinked);
	if (!isLinked)
	{
		GLint maxLength = 0;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength > 1)
		{
			//The maxLength includes the NULL character
			vector<GLchar> infoLog = vector<GLchar>(maxLength);
			glGetProgramInfoLog(*program, maxLength, NULL, &infoLog[0]);
			LOG << "OpenGL Program " << infoLog.data() << "\n";
		}
	}

	//Uniform Phase
	setUniforms();
}
void Shader::bind()
{
	string err = checkProgram(); //move check to when shader changed

	if (err == "")
	{
		glUseProgram(*program);
		setUniforms();
	}
	else
		LOG << "OpenGL Shader " << err << "\n";
}
void Shader::setUniform(const string &name, float* val)
{
	unsigned int pos = findUniform(name, 0, Uniforms.size());

	if (pos >= Uniforms.size())
		return;

	if (Uniforms[pos] == name)
		Uniforms[pos].set(val);
}
void Shader::setUniforms()
{
	for (size_t i = 0; i < Uniforms.size(); i++)
		Uniforms[i].write(*program);
}

Shader& Shader::operator=(const Shader& other)
{
	Code = other.Code;
	build();//handles the Shader changes on Program Level
	return *this;
}

Shader& Shader::operator=(const vector<ShaderCode>& Shaders)
{
	Code = Shaders;
	build();
	return *this;
}

bool Shader::operator==(const Shader& other)
{
	if (Code.size() != other.Code.size())
		return false;
	for (size_t i = 0; i < Code.size(); i++)
		if (Code[i] != other.Code[i])
			return false;
	return program.get() == other.program.get();
}

bool Shader::operator==(const ShaderCode& contained)
{
	
	ShaderCode& res = *find(Code.begin(), Code.end(), contained)._Ptr;
	return res == contained;
	return true;
}

bool Shader::operator==(const vector<ShaderCode>& Shaders)
{
	bool res = true;
	for (const ShaderCode& s : Shaders)
	{
		res = (*this == s);
		if (!res)
			break;
	}
	return res;
}

void Shader::ShaderCode::makeShader()
{
	if (!glIsShader(*_pos))
	{
		*_pos = glCreateShader(getShaderType(_type)); //no Shader existent; creating
	}
	else
	{
		int type = 0;
		glGetShaderiv(*_pos, GL_SHADER_TYPE, &type); //if there is already a Shader, check if it has the right type
		if (type != getShaderType(_type))
		{
			glDeleteShader(*_pos); //clear Shader if it is of the wrong Type (issues with Programs created from it?!)
			*_pos = glCreateShader(getShaderType(_type)); //recreate Shader of right Type
		}
	}//if there is a Shader created with the right type then it is going to be rewritten but not recreated should reduce memory
	
	if (_code.size())
	{
		const char *c_str = _code.c_str();
		glShaderSource(*_pos, 1, &c_str, NULL);
	}
	else
		LOG << "OpenGL Shader " << "Empty Source." << "\n";

	int compiled = 0;
	glGetShaderiv(*_pos, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
		glCompileShader(*_pos);

	glGetShaderiv(*_pos, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{ //not compiling
		GLint maxLength = 0;
		glGetShaderiv(*_pos, GL_INFO_LOG_LENGTH, &maxLength);
		if (maxLength > 1)
		{
			//The maxLength includes the NULL character
			vector<GLchar> infoLog = vector<GLchar>(maxLength);
			glGetShaderInfoLog(*_pos, maxLength, NULL, &infoLog[0]);
			LOG << "OpenGL Shader " << infoLog.data() << "\n";
		}
	}
}

Shader::ShaderCode& Shader::ShaderCode::operator=(const Shader::ShaderCode & other)
{
	_code = other._code;
	_owner = other._owner;
	_type = other._type;
	_path = other._path;
	uniforms = other.uniforms;
	_pos = unique_ptr<GLuint, deleteGLShader>(new GLuint(-1), deleteGLShader());
	return *this; 
}

string Shader::checkProgram()
{
	if ( *program == -1)
		return "NO PROGRAM CREATED \n";
	string errText = "";
	int res = 0;
	glValidateProgram(*program);
	glGetProgramiv(*program, GL_VALIDATE_STATUS, &res);
	if (!res)
	{
		errText += "NOT LINKED \n";
		int maxLength = 0;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		if (maxLength > 1)
		{
			string infoLog;
			infoLog.resize(maxLength);
			glGetProgramInfoLog(*program, maxLength, &maxLength, &infoLog[0]);
			errText += infoLog;
		}
	}
	return errText;
}

GLenum Shader::ShaderCode::getShaderType(ShaderType shaderType)
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

int Shader::findUniform(const string& name, int min, int max)
{
	if (min > max) {
		return min;
	}
	else if (Uniforms.size() == 0)
		return 0;
	else {
		unsigned int mid = (min + max) / 2;
		if (mid >= Uniforms.size())
			return Uniforms.size();

		if (Uniforms[mid] == name) {
			return mid;
		}
		else if (Uniforms[mid] < name) {
			return findUniform( name, mid + 1, max);
		}
		else {
			return findUniform( name, min, mid - 1);
		}
	}
}
int Shader::findUniform(const Uniform& u, int min, int max)
{
	if (min > max) {
		return min;
	}
	else if (Uniforms.size() == 0)
		return 0;
	else {
		unsigned int mid = (min + max) / 2;
		if (mid >= Uniforms.size())
			return Uniforms.size();

		if (Uniforms[mid] == u) {
			return mid;
		}
		else if (Uniforms[mid] < u) {
			return findUniform(u, mid + 1, max);
		}
		else {
			return findUniform(u, min, mid - 1);
		}
	}
}

void Shader::ShaderCode::addUniform(string & name)
{
	string uniName = "";
	if (name[name.size() - 1] == ';')
		uniName = name.substr(0, name.size() - 2);
	else
		uniName = name;
	unsigned int nameSep1 = uniName.find_first_of(' ');
	string varName = uniName.substr(nameSep1 + 1, uniName.size() - nameSep1 - 1);
	unsigned int size = Uniform::getUniformSize(name);
	if (size > 0)
		uniforms.push_back(Uniform(varName, size));
	else
	{
		vector<string> variableNames = resolveStructVariable(uniName);
		for (unsigned int i = 1; i < variableNames.size(); i++) //recursivly add all Structure Variables
		{
			string variable = "";
			unsigned int nameSep = variableNames[i].find_first_of(' ');
			variable += variableNames[i].substr(0, nameSep); //type
			variable += " " + varName;
			variable += "." + variableNames[i].substr(nameSep + 1, variableNames[i].size() - nameSep - 1);
			addUniform(variable);
		}
	}
}
vector<string> Shader::ShaderCode::resolveStructVariable(string& name)
{
	string type = name.substr(0, name.find_first_of(' '));
	for (vector<string>& _struct : structVariables)
	{
		if (_struct[0] == type)
			return _struct;
	}
	return vector<string>();
}

void Shader::Uniform::create( GLuint& prgm)
{
	pos = glGetUniformLocation(prgm, _name.c_str());
	if (pos == -1)
		LOG << "couldn't Resolve Uniform: " << _name << "\n";
}

void Shader::Uniform::write(GLuint& prgm)
{
	if(pos == -1)
		create(prgm);
	if (pos == -1 )
		return;//pos couldn't be resolved
	
	switch (_size)
	{
	case 1:
		glUniform1f(pos, _data[0]);
		break;
	case 2:
		glUniform2f(pos, _data[0], _data[0]);
		break;
	case 3:
		glUniform3f(pos, _data[0], _data[1], _data[2]);
		break;
	case 4:
		glUniform4f(pos, _data[0], _data[1], _data[2], _data[3]);
		break;
	case 9:
		glUniformMatrix3fv(pos, 1, false, _data.get());
		break;
	case 16:
		glUniformMatrix4fv(pos, 1, false, _data.get());
		break;
	default:
		break;
	}
}

int Shader::Uniform::getUniformSize(string& name)
{
	smatch result;
	if (regex_search(name, result, regex("(vec|mat)[2-4]")))
	{
		int ObjSize = atoi(&result[0].str()[3]);
		if (result[0].str()[0] == 'm') //it is a matrix
			return ObjSize * ObjSize;
		else
			return ObjSize;
	}
	else if (regex_search(name, result, regex("(sampler[1-3]D)|float")))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Shader::ShaderCode::load(ifstream &ShaderStream)
{
	string Line = "";
	if (ShaderStream.is_open())
	{
		while (getline(ShaderStream, Line))
		{
			if (ShaderStream.eof() || ShaderStream.bad())
				break;
			_code += Line + "\n";
		}
	}
	else
	{
		LOG << string(_path) + " wasn't open :/\n";
		return;
	}

	//Analyze Code
	try {
		//get Structures
		smatch _structs, _vars;

		regex structRegex = regex("struct .*?\\n?\\{\\n(.*?;\\n)*\\};");
		auto StructBegin = std::sregex_iterator(_code.begin(), _code.end(), structRegex);

		unsigned int _counter(0);
		for (auto i = StructBegin; i != std::sregex_iterator(); i++) {
			std::string temp = (*i).str();
			vector<string> structVar = vector<string>();
			regex_search(temp, _vars, regex("struct .*?\\n\\{\\n"));
			temp = _vars[0].str();
			structVar.push_back(temp.substr(7, temp.size() - 10));//adds name
			temp = (*i).str();

			regex structVarRegex = regex("\\t.*? .*?;\\n");
			auto StructVarBegin = std::sregex_iterator(temp.begin(), temp.end(), structVarRegex);

			for (std::sregex_iterator i = StructVarBegin; i != std::sregex_iterator(); ++i) {
				std::string match_str = (*i).str();
				structVar.push_back(match_str.substr(1, match_str.size() - 3)); //write the variables
			}
			structVariables.push_back(structVar);
			_counter++;
		}
		LOG << string(_path) + " contains " + to_string(_counter) + " Structures\n";

		//read uniforms
		_counter = 0;
		regex structVarRegex = regex("uniform .*? .*?;\\n");
		auto UniformsBegin = std::sregex_iterator(_code.begin(), _code.end(), structVarRegex);
		for (auto i = UniformsBegin; i != std::sregex_iterator(); i++) {
			std::string match_str = (*i).str();
			addUniform(match_str.substr(match_str.find_first_of(' ') + 1, match_str.size() - 3 - match_str.find_first_of(' ')));
			_counter++;
		}
		LOG << string(_path) + " contains " + to_string(_counter) + " Uniforms\n";
	}
	catch (regex_error &e)
	{
		LOG << string(e.what()) + "\n";
		LOG << "Could not Resolve Uniforms in Shader \n";
	}

}

void * Shader::ShaderCode::get()
{
	return this;
}