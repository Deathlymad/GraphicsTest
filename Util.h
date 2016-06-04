#pragma once

#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <queue>
#include <vector>

#include "Def.h"

#ifndef INCLUDE_UTIL

	#define NSP_UTIL using namespace util;
	NSP_STD
	NSP_CHR
	NSP_UTIL_BEG
	
		template <typename T>
		class Ptr {
		public:
			Ptr<T>() : owned(false), ptr(nullptr) {}
			Ptr<T>(T* data) : owned(true), ptr(data) {}
			Ptr<T>(T& data) : owned(true), ptr(&data)
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

			void set(T& data, int i)
			{
				ptr[i] = data;
			}

			T* get()
			{
				if (ptr)
					return ptr;
				else
					return nullptr;
			}
			T* get(unsigned int i)
			{
				if (ptr)
					return &ptr[i];
				else
					return nullptr;
			}

			bool valid(){
				return ptr != nullptr;
			}

			Ptr<T>& operator=(const Ptr<T> &other)
			{
				//takes over ownership
				setPtr( other.ptr);
				owned = false;

				return *this;
			}
			bool operator==(const Ptr<T>& other)
			{
				return ptr == other.ptr;
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

			void loseOwnership() { owned = false; }
			bool owned;
			T* ptr;
		};
	
		template <typename T>
		class CustomPtr : public Ptr<T> {
		public:
			CustomPtr<T>(function<void(T*)> func) : Ptr<T>(), destruction(func) {}
			CustomPtr<T>(function<void(T*)> func, T* data) : Ptr<T>(data), destruction(func) {}

			void setDestructor(function<void(T*)> des) { destruction = des; }
		protected:
			void setPtr(T* p)
			{
				if (owned && ptr) //if owned it clears the Data
				{
					destruction(ptr);
				}

				ptr = p;
				owned = true;
			}
		private:
			function<void(T*)> destruction;
		};

		class ThreadClient
		{
			friend class ThreadServer;
			friend class LoopedThreadServer;
		public:
			ThreadClient(function<void()> f) : _func(f), _connected(false), _executing(false)
			{}

			void disconnect() { _connected = false; }

			~ThreadClient()
			{
				_func = [] {}; //generates a clear function in order to prevent errors;
			}
		private:
			bool _connected;
			bool _executing;
			function<void()> _func;
		};
		class ThreadServer
		{
		public:
			ThreadServer(unsigned int threadCount);

			bool hasTasks();

			void addThreadClient(ThreadClient* client);

			unsigned int getThreadCount();

			~ThreadServer();
		protected:
			ThreadServer(unsigned int threadCount, function<void()> func);

			virtual void run();


			condition_variable _cond;
			mutex _condMut;
			int _update;

			mutex _listGuard;
			vector<ThreadClient*> _taskList;
			vector<future<void>> _taskResults;
			unsigned int _maxElement;
			bool _running;
		};
		class LoopedThreadServer : public ThreadServer
		{
		public:
			LoopedThreadServer(unsigned int threadCount);
		protected:
			void _run();
		};

		NSP_IO_BEG
			//iofunctions v0.1 by Linus Helfmann
			//Sonderzeichen (�,�,�,�) funktionieren nur wenn Datei in ANSI und OEM 850 kodiert
			//(Kodierung -> Zeichensatz -> Westeurop�isch -> OEM 850 in Notepad++) 
			bool strsep(vector<string> &vec, string &s);
			bool strsep(vector<string> &vec, string &s, char c);

			bool save(vector<string>  &vec, string filename);
			bool save(vector<unsigned char>&vec, string filename);
		};
	};

#define INCLUDE_UTIL
#endif