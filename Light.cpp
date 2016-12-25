#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "Shader.h"
#include "RenderingEngine.h"

NSP_GLM

BaseLight::BaseLight( vec3 c, float i, string& shaderName, string& name) : 
	color(name + ".color"),
	intensity(name + ".intensity"),
	shader("assets/shaders/" + shaderName + "_vs.glsl", "assets/shaders/" + shaderName + "_fs.glsl"),
	postShader("assets/shaders/" + shaderName + "_vs.glsl", "assets/shaders/" + shaderName + "_dl_fs.glsl")
{
	_color = c;
	_intensity = i;
}

void BaseLight::load(RessourceHandler& loader)
{
	shader.load(loader);
	postShader.load(loader);
	EngineObject::load(loader);
}

void BaseLight::init(Screen& k)
{
	shader.build();
	postShader.build();

	EngineObject::init(k);
}

void BaseLight::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		s.setUniform(color, value_ptr(_color));
		s.setUniform(intensity, &_intensity);

		EngineObject::render(s, firstPass);
		postRender(postShader);
	}
	else
		EngineObject::render(s, firstPass);
}
void BaseLight::postRender(Shader& s)
{
	//EngineObject::render(s, RenderingEngine::RenderState::POST_RENDER);
}

DirectionalLight::DirectionalLight( vec3 c, float i, vec3 dir, string& name, string& shaderName) : BaseLight( c, i, shaderName, name + ".base"), normal(name + ".direction")
{
	_normal = dir;
}

void DirectionalLight::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		s.setUniform(normal, value_ptr(_normal));
		BaseLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}

PointLight::PointLight( vec3 c, float i,Attenuation& a, vec3 p, string& name, string& shaderName) : BaseLight( c, i, shaderName, name + ".base"), pos( name + ".pos"), range( name + ".range"), atten(a), _pos(p)
{
	calcRange();
}

PointLight::PointLight(vec3 c, float i, float ex, float lin, float con, vec3 p, string& name, string& shaderName) : BaseLight(c, i, shaderName, name + ".base"), pos(name + ".pos", 3), range(name + ".range", 3), atten(con, lin, ex, name + ".atten")
{
	_pos = p;
	calcRange();
}

void PointLight::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		atten.update(s);
		s.setUniform(range, &_range);
		s.setUniform(pos, value_ptr(_pos));
		BaseLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}

SpotLight::SpotLight(vec3 c, float i, float ex, float lin, float con, vec3 p, vec3 dir, float cut, string& name, string& shaderName) : PointLight( c, i, ex, lin, con, p, name + ".point", shaderName), direction(name + ".direction"), cutoff(name + ".cutoff")
{
	_direction = normalize(dir);
	_cutoff = cut;
}

void SpotLight::render(Shader& s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		s.setUniform(direction, value_ptr(_direction));
		s.setUniform(cutoff, &_cutoff);
		PointLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}