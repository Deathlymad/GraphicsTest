#pragma once

#include <functional>

#include "Def.h"

NSP_UTIL_BEG
	
	template <typename T>
	class Ptr {
	public:
		Ptr<T>() : owned(false), ptr(nullptr) {}
		Ptr<T>(T* data) : owned(true), ptr(data) {}
		Ptr<T>(T data) : owned(true), ptr(nullptr)
		{
			ptr = new T;
			*ptr = data;
		}

		void set(T& data)
		{
			setPtr(&data);
		}
		void set(T* data)
		{
			setPtr(data);
		}

		T& get()
		{
			return *ptr;
		}

		void operator=(Ptr<T> &other)
		{
			//takes over ownership
			setPtr( other.ptr);
			other.owned = false;
			owned = true;
		}

		T& operator->()
		{
			return get();
		}

		~Ptr<T>()
		{
			setPtr(nullptr);
			owned = false;
		}
	protected:
		virtual void setPtr(T* p)
		{
			if (owned) //if owned it clears the Data
				ptr->~T();

			ptr = p;
			owned = true;
		}
		bool owned;
		T* ptr;
	};
	template <typename T>
	class CustomPtr : Ptr<T> {
	public:
		CustomPtr<T>(std::function<void(T*)> func) : Ptr<T>(), destruction(func) {}
		CustomPtr<T>(std::function<void(T*)> func, T* data) : Ptr<T>(data), destruction(func) {}
	protected:
		void setPtr(T* p)
		{
			if (owned) //if owned it clears the Data
				destrution(ptr);

			ptr = p;
			owned = true;
		}
	private:
		std::function<void(T*)> destruction;
	};
};