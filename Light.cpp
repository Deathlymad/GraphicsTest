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

void BaseLight::load(RessourceHandler * loader)
{
	shader->load(loader);
	EngineObject::load(loader);
}

void BaseLight::init(KeyMap *k)
{
	shader->build();

	this->createUniforms(_name);

	EngineObject::init(k);
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

void BaseLight::createUniforms( string& name)
{
	color = (vec3*)shader->getUniformMemPos(name + ".color");
	intensity = shader->getUniformMemPos(name + ".intensity");

	*color = _color;
	*intensity = _intensity;
}

DirectionalLight::DirectionalLight( vec3 c, float i, vec3 dir) : BaseLight( c, i)
{
	_normal = dir;
	shader = new Shader("assets/shaders/forward_directional_vs.glsl", "assets/shaders/forward_directional_fs.glsl");
	_name = "Light";
}

void DirectionalLight::createUniforms( string& name)
{
	BaseLight::createUniforms(name);
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
	shader = new Shader( "assets/shaders/forward_point_vs.glsl",  "assets/shaders/forward_point_fs.glsl");
	_name = "Light";
}
	
void PointLight::createUniforms(string& name)
{
	BaseLight::createUniforms(name + ".base");
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
	shader = new Shader( "assets/shaders/forward_spot_vs.glsl", "assets/shaders/forward_spot_fs.glsl");
	_name = "Light";
}
	
void SpotLight::createUniforms(string& name)
{
	PointLight::createUniforms(name + ".point");
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
