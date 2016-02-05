#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"
#include "Shader.h"
#include "RenderingEngine.h"

BaseLight::BaseLight( vec3 c, float i)
{
	_color = c;
	_intensity = i;
	shader = nullptr;
	_name = "";
}

void BaseLight::render(Shader * s, bool firstPass)
{
	if (s == shader)
	{
		*color = _color;
		*intensity = _intensity;
	}

	EngineObject::render(s, firstPass);
}

void BaseLight::createUniforms( string name)
{

	if (_name != name && _name.size())
	{
		shader->removeUniform(_name + ".color");
		color = nullptr;
		shader->removeUniform(_name + ".intensity");
		intensity = nullptr;
	}

	color = (vec3*)shader->getUniformMemPos(name + ".color");
	intensity = shader->getUniformMemPos(name + ".intensity");

	*color = _color;
	*intensity = _intensity;
}

DirectionalLight::DirectionalLight( vec3 c, float i, vec3 dir) : BaseLight( c, i)
{
	_normal = dir;
	shader = new Shader("forward_directional_vs.glsl", "forward_directional_fs.glsl");
	createUniforms("Light");
}

void DirectionalLight::createUniforms( string name)
{
	BaseLight::createUniforms(name);
	if (_name != name && _name.size())
	{
		shader->removeUniform(_name + ".direction");
		normal = nullptr;
	}
	normal = (vec3*)shader->getUniformMemPos(name + ".direction");

	*normal = _normal;
}

void DirectionalLight::render(Shader * s, bool firstPass)
{
	if (s == shader)
	{
		*normal = _normal;
		BaseLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}

PointLight::PointLight( vec3 c, float i,Attenuation a, vec3 p) : BaseLight( c, i)
{
	atten = a;
	_pos = p;

	float ex = a.getExponent();
	float li = a.getLinear();
	float co = a.getConstant() * getIntensity() * std::max(getColor().x, std::max(getColor().y, getColor().z));
	_range = abs((-li + sqrtf(li * li - 4 * ex * co)) / (2 * ex));
	shader = new Shader( "forward_point_vs.glsl",  "forward_point_fs.glsl");
	createUniforms("Light");
}
	
void PointLight::createUniforms(string name)
{
	BaseLight::createUniforms(name + ".base");
	if (_name != name && _name.size())
	{
		atten.createUniforms(shader, name + ".atten");
		shader->removeUniform(_name + ".range");
		range = nullptr;
		shader->removeUniform(_name + ".pos");
		pos = nullptr;
	}
	atten.createUniforms(shader, name + ".atten");
	range = shader->getUniformMemPos(name + ".range");
	pos = (vec3*)shader->getUniformMemPos(name + ".pos");

	atten.writeUniforms();
	*range = _range;
	*pos = _pos;
}

void PointLight::render(Shader * s, bool firstPass)
{
	if (s == shader)
	{
		atten.writeUniforms();
		*range = _range;
		*pos = _pos;
		BaseLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}

SpotLight::SpotLight( vec3 c, float i,Attenuation a, vec3 p, vec3 dir, float cut) : PointLight( c, i, a, p)
{
	_direction = normalize(dir);
	_cutoff = cut;
	if (shader)
		shader->~Shader();
	shader = new Shader( "forward_spot_vs.glsl", "forward_spot_fs.glsl");
	createUniforms("Light");
}
	
void SpotLight::createUniforms(string name)
{
	PointLight::createUniforms(name + ".point");
	if (_name != name && _name.size() > 0)
	{
		shader->removeUniform(_name + ".direction");
		direction = nullptr;
		shader->removeUniform(_name + ".cutoff");
		cutoff = nullptr;
	}
	direction = (vec3*)shader->getUniformMemPos(name + ".direction");
	cutoff = shader->getUniformMemPos(name + ".cutoff");

	*direction = _direction;
	*cutoff = _cutoff;
}

void SpotLight::render(Shader * s, bool firstPass)
{
	if (s == shader)
	{
		*direction = _direction;
		*cutoff = _cutoff;
		PointLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}
