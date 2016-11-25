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

	void update(float* data, unsigned size);
	void update(float data);

	UniformRegistry& operator=(const UniformRegistry& other);

	~UniformRegistry();
private:
	string _name;
	unique_ptr<float[]> _mem;
	const unsigned int _size;

	static vector<UniformRegistry*> registryList;
	unsigned int _pos;
};

