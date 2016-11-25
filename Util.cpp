#include "Util.h"
#include <stdio.h>
#include <fstream>

#include <iostream>

#include "Log.h"

NSP_UTIL_BEG
	NSP_IO_BEG

		bool strsep(vector<string> &vec, string &s)
		{
			size_t begin = 0;
			vec.clear();
			while (begin < s.length())
			{
				size_t end = begin + (s.substr(begin)).find_first_of(' ');
				if ((end >= begin) && (end != 0))
				{
					vec.push_back(s.substr(begin, end - begin));
				}
				else
				{
					vec.push_back(s.substr(begin));
					break;
				}
				begin = end + 1;
			}
			return true;
		}
		bool strsep(vector<string> &vec, string &s, char c)
		{
			unsigned int begin = 0;
			unsigned int end = 0;
			vec.clear();
			while (begin < s.size())
			{
				if (s.substr(begin).find_first_of(c) <= s.substr(begin).size())
					end = begin + (s.substr(begin).find_first_of(c));
				if ((end >= begin) && (end != 0))
				{
					vec.push_back(s.substr(begin, end - begin));
				}
				else
				{
					vec.push_back(s.substr(begin));
					break;
				}
				begin = end + 1;
			}
			return true;
		}

		//Speichern
		bool save(vector<string>  &vec, string filename)
		{
			unsigned int i = 0;
			ofstream Save(filename.c_str());
			while (Save.good() && (i < vec.size()));
			{
				Save << vec[i++] << "\n";
			}
			Save.close();
			vec.clear();
			return true;
			//Fehler abfangen???
		}
		bool save(vector<char>&vec, string filename)
		{
			ofstream Save(filename.c_str(), ios::binary | ios::trunc);
			if (Save.good())
			{
				Save.write(vec.data(), vec.size()); //Datei schreiben
				vec.clear();
			}
			if (Save.good())
			{
				Save.close();
				return true;
			}
			else
			{
				LOG << "IO Thread " << "Fehler beim Schreiben der Datei " + filename << "\n";
				Save.close();
				return false;
			}
		}
	};
};