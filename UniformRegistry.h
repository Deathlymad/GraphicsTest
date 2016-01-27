#include <vector>
#include <string>
#include "Def.h"

NSP_STD

class Shader;

#pragma once
template<int size>
class UniformRegistry
{
public:
	UniformRegistry(string name) : _name(name)
	{
		_pos = registryList.size();
		registryList.push_back(this);
	}

	void addMemPos(float * data)
	{
		MemPos.push_back(data);
	}

	void update(float* data)
	{
		for (float* pos : MemPos)
			if (pos)
				for (size_t i = 0; i < size; i++)
					pos[i] = data[i];
	}

	static void registerShaderUniforms(Shader* shader)
	{
		for (UniformRegistry* registry : registryList)
		{
			float* temp = nullptr;
			shader->addUniform(Shader::Uniform(registry->_name, temp, registry->getSize()));
			registry->addMemPos(temp);
		}
	}

	bool localized() { return !MemPos.empty(); }

	~UniformRegistry()
	{
		if (registryList.size() >= _pos)
			return;
		registryList.erase(registryList.begin() + _pos);
		for (unsigned int i = _pos; i < registryList.size(); i++)
			registryList[i]->_pos--;
	}
private:
	int getSize() { return size; }

	string _name;
	vector<float*> MemPos;

	static vector<UniformRegistry*> registryList;
	unsigned int _pos; //not working
};

template<int size>
vector<UniformRegistry<size>*> UniformRegistry<size>::registryList;

