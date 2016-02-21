#include <future>
#include <string>
#include <queue>
#include <functional>

#include "Clock.h"
#include "Util.h"

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
	RessourceLoader() { _state = PENDING; }

	virtual void load(ifstream&) = 0;
	virtual void* get() = 0;
	State getCurrentState() { return _state; }
	
	virtual ~RessourceLoader() {};
protected:
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

	void wait()
	{
		while (loading())
			this_thread::sleep_for(milliseconds(1000));
	}

	template<class T>
	shared_future<T*>* getRessource(string& file, RessourceLoader* loader)
	{
		shared_future<T*>* fut = (shared_future<T*>*)_registry.find(file);
		if (fut == nullptr)
		{
			fut = new shared_future<T*>();
			promise<T*>* t = new promise<T*>();
			*fut = t->get_future().share();
			_loadCounter++;
			loader->_state = RessourceLoader::State::PROCESSING;
			_loaderServer.addThreadClient(new ThreadClient( [ this, file, loader, t] (){
				loader->load(ifstream(file));
				T* temp = reinterpret_cast<T*>(loader->get());
				t->set_value(temp);
				_loadCounter--;
				loader->_state = RessourceLoader::State::DONE;
			}));
			_registry.push(file, new shared_future<void*>(*((shared_future<void*>*)fut)));
		}
		return ((shared_future<T*>*)_registry.find(file));
	}
	shared_future<vector<string>*>* getRessource(string& file);

	bool loading()
	{
		return _loadCounter > 1;
	}
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
		~RessourceRegistry();
	private:
		size_t find(string&, int, int);
		vector<Entry> _registry;
	} _registry;

	unsigned int _loadCounter;
	ThreadServer _loaderServer;
	static vector<string> loaderList;
};