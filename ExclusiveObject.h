#include <mutex>
#include <thread>
#include "Def.h"

NSP_STD

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
		lock = new lock_guard<mutex>(mut);
		obj = move(*o);
	}
	void unclaim(T& obj)
	{
		obj = move(*o);
		lock->~lock_guard();
	}
	void claim()
	{
		lock = new lock_guard<mutex>(mut);
	}
	void unclaim()
	{
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
	mutex mut;
	lock_guard<mutex>* lock;
};

template<typename T>
class ThreadExclusiveObject : ExclusiveObject<T> //can onlybe used by one thread at a time
{
public:
	ThreadExclusiveObject(T &obj):ExclusiveObject<T>(obj), id(this_thread::get_id())
	{}

	void claim(T &obj)
	{
		if (id != this_thread::get_id())
			return;
		ExclusiveObject<T>::claim(obj);
	}
	void unclaim(T &obj)
	{
		if (id != this_thread::get_id())
			return;
		ExclusiveObject<T>::unclaim(obj);
	}
	void claim()
	{
		if (id != this_thread::get_id())
			return;
		ExclusiveObject<T>::claim();
	}
	void unclaim()
	{
		if (id != this_thread::get_id())
			return;
		ExclusiveObject<T>::unclaim();
	}

	~ThreadExclusiveObject()
	{}
private:
	thread::id id;
};