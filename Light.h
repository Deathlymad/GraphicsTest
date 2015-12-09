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
#include <glm\common.hpp>
#include <string>
#include "EngineObject.h"
#include "Shader.h"

#pragma once
class BaseLight : public EngineObject
{
public:

	BaseLight():EngineObject(){}

	BaseLight( glm::vec3 c, float i);

	~BaseLight(void)
	{
	}
	
	glm::vec3 GetColor()
	{
		return m_color;
	}

	void SetColor(glm::vec3 color)
	{
		this->m_color = color;
	}

	float GetIntensity()
	{
		return m_intensity;
	}

	void SetIntensity(float intensity)
	{
		this->m_intensity = intensity;
	}

	virtual void writeUniform(std::string name);

	Shader* getShader(){return shader;}
protected:
	Shader* shader;
private:
	glm::vec3 m_color;
	float m_intensity;
};

#pragma once
class DirectionalLight  : public BaseLight
{
public:

	DirectionalLight() : BaseLight(){}

	DirectionalLight( glm::vec3 c, float i, glm::vec3 dir);
	
	void writeUniform( std::string name);

	void setNormal( glm::vec3 n)
	{
		normal = n;
	}

	glm::vec3 getNormal()
	{
		return normal;
	}

	~DirectionalLight()
	{
	}

private:
	glm::vec3 normal;
	static Shader* DirForwardShader; //should only create on program.get() for each type of light running off different sets of variables
};

#pragma once
class Attenuation
{
public:
	Attenuation(){};

	Attenuation( float c, float l, float e)
	{
		constant = c;
		linear = l;
		exponent = e;
	}
	
	void writeUniform( Shader* target, std::string name)
	{
		target->addUniform(Shader::Uniform::Uniform(name + ".constant", &constant, 1));
		target->addUniform(Shader::Uniform::Uniform(name + ".linear", &linear, 1));
		target->addUniform(Shader::Uniform::Uniform(name + ".exponent", &exponent, 1));
	}

	float getConstant()
	{
		return constant;
	}
	void setConstant(float c)
	{
		constant = c;
	}
	float getLinear()
	{
		return linear;
	}
	void setLinear(float c)
	{
		linear = c;
	}
	float getExponent()
	{
		return exponent;
	}
	void setExponent(float c)
	{
		exponent = c;
	}
private:
	float constant, linear, exponent;
};

#pragma once
class PointLight : public BaseLight
{
public:
	PointLight() : BaseLight(){}

	PointLight( glm::vec3 c, float i,Attenuation a, glm::vec3 p);
	
	void writeUniform(std::string name);

	void setAttenuation( Attenuation b)
	{
		atten = b;
	}
	Attenuation getAttenuation()
	{
		return atten;
	}
	void setPosition( glm::vec3 b)
	{
		pos = b;
	}
	glm::vec3 getPosition()
	{
		return pos;
	}
private:
	Attenuation atten;
	glm::vec3 pos;
	static Shader* PointForwardShader;
};

#pragma once
class SpotLight : public PointLight
{
public:
	SpotLight() : PointLight() {}
	SpotLight( glm::vec3 c, float i,Attenuation a, glm::vec3 p, glm::vec3 dir, float cut);
	
	void writeUniform(std::string name);

	glm::vec3 getDirection(){ return direction;}
	float getCutoff(){return cutoff;}

	~SpotLight(){};
	
private:
	glm::vec3 direction;
	float cutoff;
	static Shader* SpotForwardShader;
};
