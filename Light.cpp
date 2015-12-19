#include <glm/gtc/matrix_transform.hpp>

#include "Light.h"
#include "Shader.h"
#include "RenderingEngine.h"


BaseLight::BaseLight( glm::vec3 c, float i)
{
	_color = c;
	_intensity = i;
	shader = nullptr;
}

void BaseLight::render(Shader * s)
{
	if (s == shader)
	{
		*color = _color;
		*intensity = _intensity;
	}
}

void BaseLight::init(RenderingEngine * r)
{
	if (r)
		r->registerGraphicObject(this);
}

void BaseLight::createUniforms( std::string name)
{
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform( name + ".color", temp, 3));
	color = (glm::vec3*)temp;
	shader->addUniform(Shader::Uniform(name + ".intensity", intensity, 1));

	*color = _color;
	*intensity = _intensity;
}

DirectionalLight::DirectionalLight( glm::vec3 c, float i, glm::vec3 dir) : BaseLight( c, i)
{
	_normal = dir;
	shader = new Shader("forward_directional_vs.glsl", "forward_directional_fs.glsl");
}

void DirectionalLight::createUniforms( std::string name)
{
	BaseLight::createUniforms(name + ".base");
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform(name + ".direction", temp, 3));
	normal = (glm::vec3*)temp;

	*normal = _normal;
}

void DirectionalLight::render(Shader * s)
{
	if (s == shader)
	{
		BaseLight::render(s);
		*normal = _normal;
	}
}

PointLight::PointLight( glm::vec3 c, float i,Attenuation a, glm::vec3 p) : BaseLight( c, i)
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
	
void PointLight::createUniforms(std::string name)
{
	BaseLight::createUniforms(name + ".base");
	atten.createUniforms(shader, name + ".atten");
	shader->addUniform(Shader::Uniform(name + ".range", range, 1));
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform(name + ".pos", temp, 3));
	pos = (glm::vec3*)temp;

	atten.writeUniforms();
	*range = _range;
	*pos = _pos;
}

void PointLight::render(Shader * s)
{
	if (s == shader)
	{
		BaseLight::render(s);
		atten.writeUniforms();
		*range = _range;
		*pos = _pos;
	}
}

SpotLight::SpotLight( glm::vec3 c, float i,Attenuation a, glm::vec3 p, glm::vec3 dir, float cut) : PointLight( c, i, a, p)
{
	_direction = glm::normalize(dir);
	_cutoff = cut;
	if (shader)
		shader->~Shader();
	shader = new Shader( "forward_spot_vs.glsl", "forward_spot_fs.glsl");
}
	
void SpotLight::createUniforms(std::string name)
{
	PointLight::createUniforms(name + ".point");
	float* temp = nullptr;
	shader->addUniform(Shader::Uniform(name + ".direction", temp, 3));
	direction = (glm::vec3*)temp;
	shader->addUniform(Shader::Uniform(name + ".cutoff", cutoff, 1));

	*direction = _direction;
	*cutoff = _cutoff;
}

void SpotLight::render(Shader * s)
{
	if (s == shader)
	{
		PointLight::render(s);
		*direction = _direction;
		*cutoff = _cutoff;
	}
}
