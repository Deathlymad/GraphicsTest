#include "Shader.h"
#include "UniformRegistry.h"

#pragma once
class Material :public EngineObject
{
public:
	Material() : EngineObject()
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
	}

	Material( float intensity, float exponent) : EngineObject()
	{
		_specularIntensity = intensity;
		_specularExponent = exponent;
	}

	void init(Shader* prog)
	{
		float* f;
		prog->addUniform(Shader::Uniform("specularIntensity", f, 1));
		specularIntensity.addMemPos(f);
		f = nullptr;
		prog->addUniform(Shader::Uniform("specularExponent", f, 1));
		specularExponent.addMemPos(f);
	}

	void render(Shader* s)
	{
		specularIntensity.update(&_specularIntensity);
		specularExponent.update(&_specularExponent);

		EngineObject::render(s);
	}

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
	UniformRegistry<1> specularIntensity;
	float _specularExponent;
	UniformRegistry<1> specularExponent;
};