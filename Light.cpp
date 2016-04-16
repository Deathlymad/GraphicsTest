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
	shader = nullptr;
	postShader = nullptr;
}

void BaseLight::load(RessourceHandler * loader)
{
	shader->load(loader);
	if (postShader)
		postShader->load(loader);
	EngineObject::load(loader);
}

void BaseLight::init(KeyMap *k)
{
	shader->build();
	if (postShader)
		postShader->build();

	EngineObject::init(k);
}

void BaseLight::render(Shader * s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		color.update(value_ptr(_color));
		intensity.update(&_intensity);

		EngineObject::render(s, firstPass);
		postRender(postShader);
	}
	else
		EngineObject::render(s, firstPass);
}
void BaseLight::postRender(Shader* s)
{
	EngineObject::render(s, RenderingEngine::RenderState::POST_RENDER);
}

DirectionalLight::DirectionalLight( vec3 c, float i, vec3 dir, string& name) : BaseLight( c, i, name + ".base"), normal(name + ".direction", 3)
{
	_normal = dir;
	if (shader)
		shader->~Shader();
	shader = new Shader(string("assets/shaders/forward_directional_vs.glsl"), string("assets/shaders/forward_directional_fs.glsl"));
	if (postShader)
		postShader->~Shader();
	postShader = new Shader(string("assets/shaders/forward_directional_vs.glsl"), string("assets/shaders/forward_directional_dl_fs.glsl"));
}

void DirectionalLight::render(Shader * s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		normal.update(value_ptr(_normal));
		BaseLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}

PointLight::PointLight( vec3 c, float i,Attenuation a, vec3 p, string& name) : BaseLight( c, i, name + ".base"), pos( name + ".pos", 3), range( name + ".range", 3), atten(a)
{
	_pos = p;
	calcRange();
	if (shader)
		shader->~Shader();
	shader = new Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_fs.glsl"));
	if (postShader)
		postShader->~Shader();
	postShader = new Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_dl_fs.glsl"));
}

PointLight::PointLight(vec3 c, float i, float ex, float lin, float con, vec3 p, string& name) : BaseLight(c, i, name + ".base"), pos(name + ".pos", 3), range(name + ".range", 3), atten(con, lin, ex, name + ".atten")
{
	_pos = p;
	calcRange();
	if (shader)
		shader->~Shader();
	shader = new Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_fs.glsl"));
	if (postShader)
		postShader->~Shader();
	postShader = new Shader(string("assets/shaders/forward_point_vs.glsl"), string("assets/shaders/forward_point_dl_fs.glsl"));
}

void PointLight::render(Shader * s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		atten.update();
		range.update(&_range);
		pos.update(value_ptr(_pos));
		BaseLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}

SpotLight::SpotLight(vec3 c, float i, float ex, float lin, float con, vec3 p, vec3 dir, float cut, string& name) : PointLight( c, i, ex, lin, con, p, name + ".point"), direction(name + ".direction", 3), cutoff(name + ".cutoff", 1)
{
	_direction = normalize(dir);
	_cutoff = cut;
	if (shader)
		shader->~Shader();
	shader = new Shader(string("assets/shaders/forward_spot_vs.glsl"), string("assets/shaders/forward_spot_fs.glsl"));
	if (postShader)
		postShader->~Shader();
	postShader = new Shader(string("assets/shaders/forward_spot_vs.glsl"), string("assets/shaders/forward_spot_dl_fs.glsl"));
}

void SpotLight::render(Shader * s, RenderingEngine::RenderState firstPass)
{
	if (s == shader)
	{
		direction.update(value_ptr(_direction));
		cutoff.update(&_cutoff);
		PointLight::render(s, firstPass);
	}
	else
		EngineObject::render(s, firstPass);
}