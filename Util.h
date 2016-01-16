#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <queue>
#include <vector>

#include "Def.h"

#ifndef INCLUDE_UTIL

	#define NSP_UTIL using namespace util;
	NSP_STD
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

			void set(T data, int i)
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

			Ptr<T>& operator=(const Ptr<T> &other)
			{
				//takes over ownership
				setPtr( other.ptr);
				owned = false;

				return *this;
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
		public:
			ThreadClient(function<void()> f) : _func(f), _connected(false)
			{}

			void disconnect() { _connected = false; }

			~ThreadClient()
			{
				_func = [] {}; //generates a clear function in order to prevent errors;
			}
		private:
			bool _connected;
			function<void()> _func;
		};
		class ThreadServer
		{
		public:
			ThreadServer(unsigned int threadCount) : _running(true), _taskList(), _threads( new thread*[threadCount]), _conCount(threadCount)
			{
				for (unsigned j = 0; j < _conCount; j++) //constructs Threads
					_threads[_conCount] = new thread([this] {run(); });
			}

			void addThreadClient(ThreadClient* client)
			{
				client->_connected = true;
				_listGuard.lock();
				_taskList.push(client);
				_listGuard.unlock();
			}

			~ThreadServer()
			{
				_running = false;
				for (unsigned j = 0; j < _conCount; j++)
					if (_threads[j]->joinable())
						_threads[j]->join();
			}
		private:
			void run()
			{
				while (_running)
				{
					_listGuard.lock();
					if (!_taskList.size())
					{
						_listGuard.unlock();
						continue;
					}
					ThreadClient* curr = _taskList.front();
					_listGuard.unlock();

					if (!curr->_connected) //if Client disconnects the Object gets deleted
						continue;

					curr->_func();

					_listGuard.lock();
					_taskList.push(curr);
					_listGuard.unlock();
				}
			}
			mutex _listGuard;
			queue < ThreadClient*> _taskList;
			thread** _threads;
			unsigned int _conCount;
			bool _running;
		};
		NSP_IO_BEG
			//iofunctions v0.1 by Linus Helfmann
			//Sonderzeichen (ä,ö,ü,ß) funktionieren nur wenn Datei in ANSI und OEM 850 kodiert
			//(Kodierung -> Zeichensatz -> Westeuropäisch -> OEM 850 in Notepad++) 
			bool strsep(vector<string> &vec, string &s);
			bool strsep(vector<string> &vec, string &s, char c);

			bool load(vector<string>  &vec, string filename);
			bool load(vector<unsigned char>&vec, string filename);

			bool save(vector<string>  &vec, string filename);
			bool save(vector<unsigned char>&vec, string filename);
		};
	};

#define INCLUDE_UTIL
#endif