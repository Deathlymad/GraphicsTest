#include "Shader.h"

#pragma once
class Material :public EngineObject
{
public:
	Material() : EngineObject(), specularExponent("specularExponent0", 1), specularIntensity("specularIntensity0", 1)
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
		_ID = -1;
	}

	Material(unsigned int ID) : EngineObject(), specularExponent("specularExponent" + to_string(ID), 1), specularIntensity("specularIntensity" + to_string(ID), 1)
	{
		_specularExponent = 0.0f;
		_specularIntensity = 0.0f;
		_ID = ID;
	}

	Material(unsigned int ID, float intensity, float exponent) : EngineObject(), specularExponent("specularExponent" + to_string(ID), 1), specularIntensity("specularIntensity" + to_string(ID), 1)
	{
		_specularIntensity = intensity;
		_specularExponent = exponent;
		_ID = ID;
	}

	void render(Shader& s, RenderingEngine::RenderState firstPass)
	{
		s.setUniform(specularIntensity, &_specularIntensity);
		s.setUniform(specularExponent, &_specularExponent);

		EngineObject::render(s, firstPass);
	}

	void operator=(Material& m)
	{
		_specularIntensity = m._specularIntensity;
		_specularExponent = m._specularExponent;
	}

	~Material(void) {}

private:
	float _specularIntensity;
	const string specularIntensity;
	float _specularExponent;
	const string specularExponent;
	unsigned int _ID;
};