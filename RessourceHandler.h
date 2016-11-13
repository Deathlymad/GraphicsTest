#include <future>
#include <string>
#include <queue>
#include <functional>

#include "Clock.h"
#include "ThreadManager.h"
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
	RessourceLoader() : _ex(new mutex()) , _state(PENDING){}
	RessourceLoader(const RessourceLoader& other) = delete;

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
	typedef shared_future<void*> Ressource;

	RessourceHandler(ThreadManager*);

	Ressource getRessource(string& file, RessourceLoader* loader);
	
	Ressource getRessource(string& file);

private:
	class RessourceRegistry {
	public:
		struct Entry
		{
			Entry(string& s) : _obj(future<void*>()), _name(s), _checksum(-1)
			{}
			Entry(string& name, promise<void*>* fut) : _obj(future<void*>()), _name(name), _checksum(-1)
			{
				_obj = fut->get_future();
			}
			Entry(const Entry& other) : _obj(future<void*>()), _name(other._name), _checksum(-1)
			{
				_obj = other._obj;
			}

			void updateChecksum()
			{
				int k;
				unsigned int crc = -1;
				char* data = (char*)_obj.get();
				unsigned int len = sizeof(data);

				while (len--) {
					crc ^= *(data++);
					for (k = 0; k < 8; k++)
						crc = (crc & 1) ? (crc >> 1) ^ 0x82f63b78 : crc >> 1;
				}
				_checksum = crc;
			}

			bool operator==(const Entry& entry)
			{
				return _checksum == entry._checksum;
			}

			Entry& operator=(const Entry& other)
			{
				_name = other._name;
				_obj = other._obj;
				_checksum = other._checksum;
				return *this;
			}

			unsigned int _checksum;
			string _name;
			Ressource _obj;
		};
		Ressource find(string&);

		void push(Entry);
		void push( string, promise<void*>*);

		int loading()
		{
			unsigned counter = 0;
			for (Entry& e : _registry)
				if ( (e._obj.wait_for(seconds(0)) != _Future_status::ready))
					counter++;
			return counter;
		}

		~RessourceRegistry();
	private:
		size_t find(string&, int, int);
		vector<Entry> _registry;
	} _registry;

	ThreadManager* _taskManager;
	unsigned int _loadCounter;
	static vector<string> loaderList;
};