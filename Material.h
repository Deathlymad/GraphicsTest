#include "Shader.h"

#pragma once
class Material :public EngineObject
{
public:
	Material() : EngineObject(), specularIntensity(nullptr), specularExponent(nullptr)
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
	}

	Material( float intensity, float exponent) : EngineObject(), specularIntensity(nullptr), specularExponent(nullptr)
	{
		_specularIntensity = intensity;
		_specularExponent = exponent;
	}

	void createUniforms(Shader* prog)
	{
		prog->addUniform(Shader::Uniform("specularIntensity", specularIntensity, 1));
		prog->addUniform(Shader::Uniform("specularExponent", specularExponent, 1));
	}

	void render(Shader* s)
	{
		if (isLoaded())
		{
			*specularIntensity = _specularIntensity;
			*specularExponent = _specularExponent;
		}
		else
		{
			createUniforms(s);
		}
	}

	bool isLoaded() { return specularIntensity && specularExponent; }

	void operator=(Material& m)
	{
		specularIntensity = m.specularIntensity;
		specularExponent = m.specularExponent;
		_specularIntensity = m._specularIntensity;
		_specularExponent = m._specularExponent;
	}

	~Material(void) {}

private:
	float _specularIntensity;
	float* specularIntensity;
	float _specularExponent;
	float* specularExponent;
};