#include "Shader.h"
#include "UniformRegistry.h"

#pragma once
class Material :public EngineObject
{
public:
	Material() : EngineObject(), specularExponent("specularExponent"), specularIntensity("specularIntensity")
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
		_ID = -1;
	}

	Material(unsigned int ID) : EngineObject(), specularExponent("specularExponent" + to_string(ID)), specularIntensity("specularIntensity" + to_string(ID))
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
		_ID = ID;
	}

	Material(unsigned int ID, float intensity, float exponent) : EngineObject(), specularExponent("specularExponent" + to_string(ID)), specularIntensity("specularIntensity" + to_string(ID))
	{
		_specularIntensity = intensity;
		_specularExponent = exponent;
		_ID = ID;
	}

	void render(Shader* s, bool firstPass)
	{
		specularIntensity.update(&_specularIntensity);
		specularExponent.update(&_specularExponent);

		EngineObject::render(s, firstPass);
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