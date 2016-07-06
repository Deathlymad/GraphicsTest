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

		//Provided by L Peter Deutsch under the zlib license
		NSP_MD5_BEG
			/* Define the state of the MD5 Algorithm. */
			typedef struct md5_state_s {
				unsigned int count[2];	/* message length in bits, lsw first */
				unsigned int abcd[4];		/* digest buffer */
				unsigned char buf[64];		/* accumulate block */
			} md5_state_t;
		
			/* Initialize the algorithm. */
			void md5_init(md5_state_t *pms);

			/* Append a string to the message. */
			void md5_append(md5_state_t *pms, const unsigned char *data, int nbytes);

			/* Finish the message and return the digest. */
			void md5_finish(md5_state_t *pms, unsigned char digest[16]);
		};

		NSP_IO_BEG
			//iofunctions v0.1 by Linus Helfmann
			//Sonderzeichen (ä,ö,ü,ß) funktionieren nur wenn Datei in ANSI und OEM 850 kodiert
			//(Kodierung -> Zeichensatz -> Westeuropäisch -> OEM 850 in Notepad++) 
			bool strsep(vector<string> &vec, string &s);
			bool strsep(vector<string> &vec, string &s, char c);

			bool save(vector<string>  &vec, string filename);
			bool save(vector<unsigned char>&vec, string filename);
		};
	};

#define INCLUDE_UTIL
#endif