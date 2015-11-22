#include <mutex>
#include <thread>

#pragma once
template<typename T>
class ExclusiveObject //can onlybe used by one thread at a time
{
public:
	ExclusiveObject(...)
	{
		o = new T(...);
		owned = true;
	}
	ExclusiveObject(T &obj) : o(&obj)
	{
		owned = false;
	}

	void claim(T& obj)
	{
		lock = new std::lock_guard<std::mutex>(mut);
		//obj = std::move(*o);
	}
	void unclaim(T& obj)
	{
		//obj = std::move(*o);
		lock->~lock_guard();
	}

	~ExclusiveObject()
	{
		if (owned)
			o->~T();
	}
private:
	bool owned;
	T* o;
	std::mutex mut;
	std::lock_guard<std::mutex>* lock;
};

template<typename T>
class ThreadExclusiveObject : ExclusiveObject<T> //can onlybe used by one thread at a time
{
public:
	ThreadExclusiveObject(T &obj):ExclusiveObject<T>(obj), id(std::this_thread::get_id())
	{}

	void claim(T &obj)
	{
		if (id != std::this_thread::get_id())
			return;
		ExclusiveObject<T>::claim(obj);
	}
	void unclaim(T &obj)
	{
		if (id != std::this_thread::get_id())
			return;
		ExclusiveObject<T>::unclaim(obj);
	}

	~ThreadExclusiveObject()
	{}
private:
	std::thread::id id;
};