#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Light.h"
#include "Shader.h"
#include "RenderingEngine.h"

NSP_GLM

BaseLight::BaseLight( vec3 c, float i, string& name) : color(name + ".color", 3), intensity(name + ".intensity", 1)
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

void BaseLight::init(KeyMap& k)
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

DirectionalLight::DirectionalLight( vec3 c, float i, vec3 dir, string& name) : BaseLight( c, i, name + ".base"), normal(name + ".direction", 3)
{
	_normal = dir;
	shader = Shader(string("assets/shaders/forward_directional_vs.glsl"), string("assets/shaders/forward_directional_fs.glsl"));
	postShader = Shader(string("assets/shaders/forward_directional_vs.glsl"), string("assets/shaders/forward_directional_dl_fs.glsl"));
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

PointLight::PointLight( vec3 c, float i,Attenuation& a, vec3 p, string& name) : BaseLight( c, i, name + ".base"), pos( name + ".pos", 3), range( name + ".range", 3), atten(a), _pos(p)
{
	calcRange();
	shader = Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_fs.glsl"));
	postShader = Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_dl_fs.glsl"));
}

PointLight::PointLight(vec3 c, float i, float ex, float lin, float con, vec3 p, string& name) : BaseLight(c, i, name + ".base"), pos(name + ".pos", 3), range(name + ".range", 3), atten(con, lin, ex, name + ".atten")
{
	_pos = p;
	calcRange();
	shader = Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_fs.glsl"));
	postShader = Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_dl_fs.glsl"));
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

SpotLight::SpotLight(vec3 c, float i, float ex, float lin, float con, vec3 p, vec3 dir, float cut, string& name) : PointLight( c, i, ex, lin, con, p, name + ".point"), direction(name + ".direction", 3), cutoff(name + ".cutoff", 1)
{
	_direction = normalize(dir);
	_cutoff = cut;
	shader = Shader(string("assets/shaders/forward_spot_vs.glsl"), string("assets/shaders/forward_spot_fs.glsl"));
	postShader = Shader(string("assets/shaders/forward_spot_vs.glsl"), string("assets/shaders/forward_spot_dl_fs.glsl"));
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