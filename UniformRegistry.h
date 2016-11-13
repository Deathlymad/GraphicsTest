#include <vector>
#include <string>
#include "Def.h"

class Shader;

NSP_STD

#pragma once
class UniformRegistry
{
public:
	UniformRegistry(const char* str, unsigned int size);
	UniformRegistry(string& name, unsigned int size);

	void addMemPos(float * data);

	void update(float* data);

	static void registerShaderUniforms(Shader& shader);

	bool localized();

	UniformRegistry& operator=(const UniformRegistry& other);

	~UniformRegistry();
private:
	string _name;
	vector<float*> MemPos;
	const unsigned int _size;

	static vector<UniformRegistry*> registryList;
	unsigned int _pos;
};

