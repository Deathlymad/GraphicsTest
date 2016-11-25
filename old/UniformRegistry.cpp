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

void UniformRegistry::update(float* data, unsigned size)
{
	if ( size == sizeof(_mem.get()))
		memcpy( _mem.get(), data, size);
}

void UniformRegistry::update(float data)
{
	*_mem.get() = data;
}

UniformRegistry::~UniformRegistry()
{
	if (registryList.size() >= _pos)
		return;
	registryList.erase(registryList.begin() + _pos);
	for (unsigned int i = _pos; i < registryList.size(); i++)
		registryList[i]->_pos--;
}