#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"
#include "Shader.h"
#include "RenderingEngine.h"


BaseLight::BaseLight( vec3 c, float i)
{
	_color = c;
	_intensity = i;
	shader = nullptr;
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

void BaseLight::init(RenderingEngine * r)
{
	if (r)
		r->registerGraphicObject(this);
}

void BaseLight::createUniforms( string name)
{
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform( name + ".color", temp, 3));
	color = (vec3*)temp;
	shader->addUniform(Shader::Uniform(name + ".intensity", intensity, 1));

	*color = _color;
	*intensity = _intensity;
}

DirectionalLight::DirectionalLight( vec3 c, float i, vec3 dir) : BaseLight( c, i)
{
	_normal = dir;
	shader = new Shader("forward_directional_vs.glsl", "forward_directional_fs.glsl");
}

void DirectionalLight::createUniforms( string name)
{
	BaseLight::createUniforms(name + ".base");
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform(name + ".direction", temp, 3));
	normal = (vec3*)temp;

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
	float li = a.getLinear() / ex;
	float co = a.getConstant() / ex;
	if (((li * li) / 4 - co) > 0)
		_range = (-li / 2 + sqrt((li * li) / 4 - co)); //calculate point where equation returns 0
	else
		_range = 1000.0f;
	shader = new Shader( "forward_point_vs.glsl",  "forward_point_fs.glsl");
}
	
void PointLight::createUniforms(string name)
{
	BaseLight::createUniforms(name + ".base");
	atten.createUniforms(shader, name + ".atten");
	shader->addUniform(Shader::Uniform(name + ".range", range, 1));
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform(name + ".pos", temp, 3));
	pos = (vec3*)temp;

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
}
	
void SpotLight::createUniforms(string name)
{
	PointLight::createUniforms(name + ".point");
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform(name + ".direction", temp, 3));
	direction = (vec3*)temp;
	shader->addUniform(Shader::Uniform(name + ".cutoff", cutoff, 1));

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
