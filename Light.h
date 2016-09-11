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
#include"UniformRegistry.h"

NSP_STD
NSP_GLM

class RenderingEngine;

#pragma once
class BaseLight :public EngineObject
{
public:
	BaseLight( vec3 c, float i, string& name = string("Light"));
	
	virtual void load(RessourceHandler* loader);
	virtual void init(KeyMap*);

	float getIntensity() { return _intensity; }
	vec3 getColor() { return _color; }

	virtual void render(Shader*, RenderingEngine::RenderState);
	virtual void postRender(Shader*);
	void render() { this->render(shader, RenderingEngine::RenderState::FWD_RENDER); }

	~BaseLight(void)
	{
	}
	
	Shader* getShader(){return shader;}
protected:
	Shader* shader;
	Shader* postShader;
private:
	vec3 _color;
	UniformRegistry color;
	float _intensity;
	UniformRegistry intensity;
};

#pragma once
class DirectionalLight  : public BaseLight
{
public:
	DirectionalLight( vec3 c, float i, vec3 dir, string& name = string("Light"));

	virtual void render(Shader*, RenderingEngine::RenderState);

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
	vec3 _normal;
	UniformRegistry normal;
};

#pragma once
class Attenuation
{
public:
	Attenuation( float c, float l, float e, string& name) : constant(name + ".constant", 1), linear(name + ".linear", 1), exponent(name + ".exponent", 1)
	{
		_constant = c;
		_linear = l;
		_exponent = e;
	}
	void update()
	{
		constant.update(&_constant);
		linear.update(&_linear);
		exponent.update(&_exponent);
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
	UniformRegistry constant, linear, exponent;
};

#pragma once
class PointLight : public BaseLight
{
public:
	PointLight( vec3 c, float i,Attenuation& a, vec3 p, string& name = string("Light"));
	PointLight(vec3 c, float i, float ex, float lin, float con, vec3 p, string& name = string("Light"));
	
	virtual void render(Shader*, RenderingEngine::RenderState);

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
	void calcRange()
	{
		float ex = atten.getExponent();
		float li = atten.getLinear();
		float co = atten.getConstant() * getIntensity() * std::max(getColor().x, std::max(getColor().y, getColor().z));
		_range = abs((-li + sqrtf(li * li - 4 * ex * co)) / (2 * ex));
		if ( isnan(_range))
			_range = 100.0f;
	}

	Attenuation atten;
	vec3 _pos;
	UniformRegistry pos;
	float _range;
	UniformRegistry range;
};

#pragma once
class SpotLight : public PointLight
{
public:
	SpotLight( vec3 c, float i, float ex, float lin, float con, vec3 p, vec3 dir, float cut, string& name = string("Light"));
	
	virtual void render(Shader*, RenderingEngine::RenderState);

	vec3 getDirection(){ return _direction;}
	float getCutoff(){return _cutoff;}

	~SpotLight(){};
	
private:
	vec3 _direction;
	UniformRegistry direction;
	float _cutoff;
	UniformRegistry cutoff;
};
