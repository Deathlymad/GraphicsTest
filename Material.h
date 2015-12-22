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
		_ID = -1;
	}

	Material(unsigned int ID) : EngineObject()
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
		_ID = ID;
	}

	Material(unsigned int ID, float intensity, float exponent) : EngineObject()
	{
		_specularIntensity = intensity;
		_specularExponent = exponent;
		_ID = ID;
	}

	void init(Shader* prog)
	{
		float* f;
		if (_ID != -1)
			prog->addUniform(Shader::Uniform("specularIntensity" + std::to_string(_ID), f, 1));
		else
			prog->addUniform(Shader::Uniform("specularIntensity", f, 1));
		specularIntensity.addMemPos(f);
		f = nullptr;
		if (_ID != -1)
			prog->addUniform(Shader::Uniform("specularExponent" + std::to_string(_ID), f, 1));
		else
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
	unsigned int _ID;
};