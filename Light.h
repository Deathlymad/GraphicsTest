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
#include "Def.h"

NSP_STD
NSP_GLM

class RenderingEngine;

#pragma once
class BaseLight :public EngineObject
{
public:

	BaseLight() {}

	BaseLight( vec3 c, float i);

	virtual void render(Shader*);
	void render() { this->render(shader); }

	~BaseLight(void)
	{
	}
	
	void init(RenderingEngine*);

	virtual void createUniforms(string name);

	Shader* getShader(){return shader;}
protected:
	Shader* shader;
private:
	vec3 _color, *color;
	float _intensity, *intensity;
};

#pragma once
class DirectionalLight  : public BaseLight
{
public:

	DirectionalLight() : BaseLight(){}

	DirectionalLight( vec3 c, float i, vec3 dir);
	
	virtual void createUniforms( string name);

	virtual void render(Shader*);

	void setNormal( vec3 n)
	{
		_normal = n;
	}

	vec3 getNormal()
	{
		return _normal;
	}

	~DirectionalLight()
	{
	}

private:
	vec3 _normal, *normal;
};

#pragma once
class Attenuation
{
public:
	Attenuation(){};

	Attenuation( float c, float l, float e)
	{
		_constant = c;
		_linear = l;
		_exponent = e;
	}
	
	void createUniforms( Shader* target, string name)
	{
		target->addUniform(Shader::Uniform::Uniform(name + ".constant", constant, 1));
		target->addUniform(Shader::Uniform::Uniform(name + ".linear", linear, 1));
		target->addUniform(Shader::Uniform::Uniform(name + ".exponent", exponent, 1));
	}
	void writeUniforms()
	{
		*constant = _constant;
		*linear = _linear;
		*exponent = _exponent;
	}

	float getConstant()
	{
		return _constant;
	}
	void setConstant(float c)
	{
		_constant = c;
	}
	float getLinear()
	{
		return _linear;
	}
	void setLinear(float c)
	{
		_linear = c;
	}
	float getExponent()
	{
		return _exponent;
	}
	void setExponent(float c)
	{
		_exponent = c;
	}
private:
	float _constant, _linear, _exponent;
	float* constant, *linear, *exponent;
};

#pragma once
class PointLight : public BaseLight
{
public:
	PointLight() : BaseLight(){}

	PointLight( vec3 c, float i,Attenuation a, vec3 p);
	
	virtual void createUniforms(string name);

	virtual void render(Shader*);

	void setAttenuation( Attenuation b)
	{
		atten = b;
		float ex = atten.getExponent();
		float li = atten.getLinear() / ex;
		float co = atten.getConstant() / ex;
		if (((li * li) / 4 - co) > 0)
			_range = (-li / 2 + sqrt((li * li) / 4 - co)); //calculate point where equation returns 0
		else
			_range = 1000.0f;
	}

	Attenuation getAttenuation()
	{
		return atten;
	}
	void setPosition( vec3 b)
	{
		_pos = b;
	}
	vec3 getPosition()
	{
		return _pos;
	}
private:
	Attenuation atten;
	vec3 _pos, *pos;
	float _range, *range;
};

#pragma once
class SpotLight : public PointLight
{
public:
	SpotLight() : PointLight() {}
	SpotLight( vec3 c, float i,Attenuation a, vec3 p, vec3 dir, float cut);
	
	virtual void createUniforms(string name);

	virtual void render(Shader*);

	vec3 getDirection(){ return _direction;}
	float getCutoff(){return _cutoff;}

	~SpotLight(){};
	
private:
	vec3 _direction, *direction;
	float _cutoff, *cutoff;
};
