#include "RessourceHandler.h"

#include <fstream>

vector<string> RessourceHandler::loaderList = vector<string>();

RessourceHandler::RessourceHandler(ThreadManager* manager) : _loadCounter(0), _taskManager(manager)
{
}

RessourceHandler::Ressource RessourceHandler::getRessource(string & file, RessourceLoader * loader)
{
	Ressource fut = _registry.find(file);
	if (!fut.valid())
	{
		_loadCounter++;
		loader->_state = RessourceLoader::State::PROCESSING;
			promise<void*>* p = new promise<void*>(); //mem Leak?
			_registry.push(file, p);
		LOG << string("Started Loading ") + file + " \n";
		_taskManager->addTask([this, file, loader, p]() {
			LOG << string("Started Custom Loader ") + file + " \n";
			lock_guard<mutex>(*loader->_ex);
			loader->load(ifstream(file));

			p->set_value((void**)loader->get()); //invalid O.o
			_loadCounter--;
			loader->_state = RessourceLoader::State::DONE;
			return 0;
		});
	}
	return _registry.find(file);
}

RessourceHandler::Ressource RessourceHandler::getRessource(string& file)
{
	STDTextLoader _loader;
	return (Ressource)getRessource(file, &_loader);
}

RessourceHandler::Ressource RessourceHandler::RessourceRegistry::find(string& name)
{
	unsigned int pos = find(name, 0, _registry.size());
	if (pos >= _registry.size())
		return Ressource();
	if (_registry[pos]._name == name)
		return _registry[pos]._obj;
	return Ressource();
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

void RessourceHandler::RessourceRegistry::push( string name, promise<void*>* fut)
{
	return push(Entry(name, fut));
}

RessourceHandler::RessourceRegistry::~RessourceRegistry()
{
	for (Entry& e : _registry)
	{
		e._obj.~shared_future();
	}
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
