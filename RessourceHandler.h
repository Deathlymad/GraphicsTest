#include <future>
#include <string>
#include <queue>
#include <functional>

#include "Clock.h"
#include "Util.h"

#include "Log.h"

NSP_STD
NSP_UTIL

#pragma once
class RessourceLoader //Loader Interface
{
	enum State {
		PENDING,
		PROCESSING,
		DONE
	};
	friend class RessourceHandler;
public:
	RessourceLoader() : _ex(new mutex()) { _state = PENDING; }

	virtual void load(ifstream&) = 0;
	virtual void* get() = 0;
	State getCurrentState() { return _state; }
	
	virtual ~RessourceLoader() {};
protected:
	mutex* _ex;
	State _state;
};

class RessourceHandler
{
	class STDTextLoader : public RessourceLoader
	{
	public:
		void load(ifstream&);
		void* get() { return (void*)&_buffer; }
	private:
		vector<string> _buffer;
	};
public:
	RessourceHandler();

	template<class T>
	shared_future<T*>* getRessource(string& file, RessourceLoader* loader)
	{
		shared_future<T*>* fut = (shared_future<T*>*)_registry.find(file);
		if (fut == nullptr)
		{
			promise<T*>* t = new promise<T*>();
			_loadCounter++;
			loader->_state = RessourceLoader::State::PROCESSING;
			LOG << string("Started Loading ") + file + " \n";
			_loaderServer.addThreadClient(new ThreadClient( [this, file, loader, t] (){
				LOG << string("Started Custom Loader ") + file + " \n";
				lock_guard<mutex>(*loader->_ex);
				loader->load(ifstream(file));
				t->set_value(reinterpret_cast<T*>(loader->get()));
				_loadCounter--;
				loader->_state = RessourceLoader::State::DONE;
			}));
			_registry.push(file, new shared_future<void*>(*reinterpret_cast<shared_future<void*>*>(&t->get_future().share())));
		}
		return ((shared_future<T*>*)_registry.find(file));
	}
	shared_future<vector<string>*>* getRessource(string& file);

private:
	class RessourceRegistry {
	public:
		struct Entry
		{
			Entry(string s)
			{
				_name = s;
				_obj = new shared_future<void*>();
			}
			Entry(string s, shared_future<void*>* obj) : _obj(obj)
			{
				_name = s;
			}
			string _name;
			shared_future<void*>* _obj;
		};
		shared_future<void*>* find(string&);

		void push(Entry);
		void push( string, shared_future<void*>*);

		int loading()
		{
			unsigned counter = 0;
			for (Entry& e : _registry)
				if (!e._obj->_Is_ready())
					counter++;
			return counter;
		}

		~RessourceRegistry();
	private:
		size_t find(string&, int, int);
		vector<Entry> _registry;
	} _registry;

	unsigned int _loadCounter;
	ThreadServer _loaderServer;
	static vector<string> loaderList;
};