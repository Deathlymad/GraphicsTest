#include "UniformRegistry.h"
#include "Shader.h"
#include "Log.h"

vector<UniformRegistry*> UniformRegistry::registryList;


UniformRegistry::UniformRegistry(const char* name, unsigned int size) : _name(name), _size(size)
{
	_pos = registryList.size();
	registryList.push_back(this);
}

UniformRegistry::UniformRegistry(string& name, unsigned int size) : _name(name), _size(size)
{
	_pos = registryList.size();
	registryList.push_back(this);
}

void UniformRegistry::addMemPos(float * data)
{
	MemPos.push_back(data);
}

void UniformRegistry::update(float* data)
{
	for (float* pos : MemPos)
		if (pos && data)
			for (size_t i = 0; i < _size; i++)
				pos[i] = data[i];
}

void UniformRegistry::registerShaderUniforms(Shader& shader)
{
	for (UniformRegistry* registry : registryList)
		registry->addMemPos(shader.getUniformMemPos(registry->_name));
}

bool UniformRegistry::localized() { return !MemPos.empty(); }

UniformRegistry& UniformRegistry::operator=(const UniformRegistry& other)
{
	if (other._size != _size)
		return *this; //can't be copied
	_name = other._name;
	MemPos = other.MemPos;
	return *this;
}

UniformRegistry::~UniformRegistry()
{
	if (registryList.size() >= _pos)
		return;
	registryList.erase(registryList.begin() + _pos);
	for (unsigned int i = _pos; i < registryList.size(); i++)
		registryList[i]->_pos--;
}