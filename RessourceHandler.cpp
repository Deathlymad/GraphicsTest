#include "RessourceHandler.h"

#include <fstream>

vector<string> RessourceHandler::loaderList = vector<string>();

RessourceHandler::RessourceHandler() : LoadCounter(0)
{
}

shared_future<vector<string>*> RessourceHandler::getRessource(string& file)
{
	STDTextLoader _loader;
	return getRessource<vector<string>>(file, &_loader);
}

shared_future<void*>* RessourceHandler::RessourceRegistry::find(string& name)
{
	unsigned int pos = find(name, 0, _registry.size());
	if (pos >= _registry.size())
		return nullptr;
	if (_registry[pos]._name == name)
		return _registry[pos]._obj;
	return nullptr;
}

size_t RessourceHandler::RessourceRegistry::find(string& name, int min, int max)
{
	if (min > max) {
		return min;
	}
	else if (_registry.size() == 0)
		return 0;
	else {
		unsigned int mid = (min + max) / 2;
		if (mid >= _registry.size())
			return _registry.size();
		int comp = _registry[mid]._name.compare(name);
		if (comp == 0) {
			return mid;
		}
		else if (comp < 0) {
			return find(name, mid + 1, max);
		}
		else {
			return find(name, min, mid - 1);
		}
	}
	return 0;
}

void RessourceHandler::RessourceRegistry::push(Entry e)
{
	_registry.insert(_registry.begin() + find(e._name, 0, _registry.size()), e);
}

void RessourceHandler::RessourceRegistry::push( string name, shared_future<void*>* data)
{
	push(Entry(name, data));
}

RessourceHandler::RessourceRegistry::~RessourceRegistry()
{
	for (Entry& e : _registry)
		e._obj->~shared_future();
}

void RessourceHandler::STDBinaryLoader::load(ifstream &Fstream)
{
	string fileCode;
	//Read Code from File
	if (Fstream.is_open())
	{
		string Line = "";
		while (getline(Fstream, Line))
		{
			fileCode += Line + "\n";
		}
		strcpy(_buffer, fileCode.c_str());
	}
	else
		_buffer = nullptr;
}

void RessourceHandler::STDTextLoader::load(ifstream &f)
{
	_buffer.clear();
	if (f.is_open())
	{
		string Line = "";
		while (getline(f, Line))
		{
			_buffer.push_back(Line);
		}
	}
}
