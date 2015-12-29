#pragma once

#include <functional>
#include <string>
#include <vector>

#include "Def.h"

#ifndef INCLUDE_UTIL

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

		NSP_IO_BEG
			//iofunctions v0.1 by Linus Helfmann
			//Sonderzeichen (ä,ö,ü,ß) funktionieren nur wenn Datei in ANSI und OEM 850 kodiert
			//(Kodierung -> Zeichensatz -> Westeuropäisch -> OEM 850 in Notepadd++) 
			//Hilfsfunktionen (verschieben?) ja, util
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