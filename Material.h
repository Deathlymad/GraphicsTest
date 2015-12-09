#include "Shader.h"

#pragma once
class Material
{
public:
	Material()
	{
		specularExponent = 0.0f;
		specularIntensity = 0.0f;
		loaded = false;
	}

	Material( float intensity, float exponent)
	{
		loaded = false;
		specularIntensity = intensity;
		specularExponent = exponent;
	}

	void createUniforms(Shader* prog)
	{
		prog->addUniform(Shader::Uniform("specularIntensity", &specularIntensity, 1));
		prog->addUniform(Shader::Uniform("specularExponent", &specularExponent, 1));
		loaded = true;
	}

	bool isLoaded() { return loaded; }

	void operator=(Material m)
	{
		specularIntensity = m.specularIntensity;
		specularExponent = m.specularExponent;
	}

	~Material(void) {}

private:
	bool loaded;
	float specularIntensity;
	float specularExponent;
};