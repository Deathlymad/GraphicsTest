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

	template<class T>
	shared_future<T*>* getRessource(string& file, RessourceLoader* loader)
	{
		shared_future<T*>* fut = (shared_future<T*>*)_registry.find(file);
		if (fut == nullptr)
		{
			fut = new shared_future<T*>();
			*fut = async([this] (RessourceHandler* handler, string path, RessourceLoader* _loader) -> T* {
				return (
					handler->LoadCounter++,
					_loader->load(ifstream(path)),
					handler->LoadCounter--,
					reinterpret_cast<T*>(_loader->get())
					);
			}, this, file, loader).share();
			_registry.push(file, new shared_future<void*>(*((shared_future<void*>*)fut)));
		}
		return ((shared_future<T*>*)_registry.find(file));
	}
	shared_future<vector<string>*>* getRessource(string& file);

	bool loading() { return LoadCounter > 0; }
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

	unsigned int LoadCounter;

	static vector<string> loaderList;
};