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