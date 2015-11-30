#include <glm\gtc\matrix_transform.hpp>

#include "Light.h"
#include "Shader.h"


BaseLight::BaseLight( glm::vec3 c, float i) : EngineObject()
{
	m_color = c;
	m_intensity = i;
	shader = nullptr;
}

void BaseLight::writeUniform( std::string name)
{
	shader->addUniform(Shader::Uniform( name + ".color", &m_color[0], 3));
	shader->addUniform(Shader::Uniform(name + ".intensity", &m_intensity, 1));
}

DirectionalLight::DirectionalLight( glm::vec3 c, float i, glm::vec3 dir) : BaseLight( c, i)
{
	normal = dir;
	if (!DirForwardShader)
		DirForwardShader =new Shader( "forward_directional_vs.glsl", "forward_directional_fs.glsl");
	shader = DirForwardShader;
}

void DirectionalLight::writeUniform( std::string name)
{
	BaseLight::writeUniform(name + ".base");
	shader->addUniform(Shader::Uniform(name + ".direction", &normal[0], 3));
}

PointLight::PointLight( glm::vec3 c, float i,Attenuation a, glm::vec3 p) : BaseLight( c, i)
{
	atten = a;
	pos = p;
	if (!PointForwardShader)
		PointForwardShader = new Shader( "forward_point_vs.glsl",  "forward_point_fs.glsl");
	shader = PointForwardShader;
}
	
void PointLight::writeUniform(std::string name)
{
	BaseLight::writeUniform(name + ".base");
	atten.writeUniform(shader, name + ".atten");
	shader->addUniform(Shader::Uniform(name + ".pos", &pos[0], 3));
}

SpotLight::SpotLight( glm::vec3 c, float i,Attenuation a, glm::vec3 p, glm::vec3 dir, float cut) : PointLight( c, i, a, p)
{
	direction = glm::normalize(dir);
	cutoff = cut;
	if (!SpotForwardShader)
		SpotForwardShader = new Shader( "forward_spot_vs.glsl", "forward_spot_fs.glsl");
	shader = SpotForwardShader;
}
	
void SpotLight::writeUniform(std::string name)
{
	PointLight::writeUniform(name + ".point");
	shader->addUniform(Shader::Uniform(name + ".direction", &direction[0], 3));
	shader->addUniform(Shader::Uniform(name + ".cutoff", &cutoff, 1));
}

Shader* DirectionalLight::DirForwardShader = nullptr;
Shader* PointLight::PointForwardShader = nullptr;
Shader* SpotLight::SpotForwardShader = nullptr;