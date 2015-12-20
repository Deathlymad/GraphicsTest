#include "Util.h"
#include <stdio.h>
#include <fstream>
#include <iostream>

NSP_UTIL_BEG
	NSP_IO_BEG

		bool strsep(vector<string> &vec, string &s)
		{
			unsigned int begin = 0, end = 0;
			vec.clear();
			while (begin<s.length())
			{
				end = begin + (s.substr(begin)).find_first_of(' ');
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
			while (begin<s.size())
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

		//Laden
		bool load(vector<string>  &vec, string filename)
		{
			string temp;
			vec.clear();
			ifstream Text(filename.c_str());

			if (!Text)
				Text.open(filename.c_str(), ios::in);
			//File doesn't exists
			while (Text.good())
			{
				getline(Text, temp);
				if ((temp.length() > 1) && (temp[0] == '#') && (temp[1] == '#')) //Dateiende
				{
					break;
				}
				if ((temp.length() > 1) && (temp[0] != '#') && !((temp.length() <= 1) && !isalnum(temp[0])))
				{						//wenn kein Kommentar oder Leerzeile (Kommentare merken? sie werden sp�ter vielleicht gebraucht um Bereiche zu erkennen)				
					vec.push_back(temp);//oder man macht eine Zeile wo ein Schl�sselwort steht z.b. "Eigenschaften" oder man macht schreibt immer "break"???
				}						//Die Zeilen k�nnen noch am Ende ein Kommentar enthalten!!!
			}
			Text.close();

			vec.resize(vec.size()); //�berfl�ssig bereitgestellter Speicher wird gel�scht

			if ((vec.size() == 0))
			{
				cout << "IO Handler " << "Fehler beim Laden der Datei " + filename << endl;
				return false;
			}
			return true;
		}
		bool load(vector<unsigned char>&vec, string filename)
		{
			unsigned int size;
			char * buffer;
			vec.clear();
			ifstream Load(filename.c_str(), ios::binary | ios::ate);
			if (Load.good())
			{
				size = (unsigned int)Load.tellg(); //ate => Ende der Datei, tellg gibt die Position zur�ck und damit die L�nge der Datei!
												   //Dateien immer in einem St�ck lesen? oder irgentwann (z.b. ab ein paar KB) zerst�ckeln (mit Threads)
												   //das w�hre beim Laden und Verarbeiten von gro�en Dateien hilfreich, z.b. bei Objekten
				Load.seekg(0, ios::beg); //position wird wieder auf Anfang gesetzt
				buffer = new char[size]; //char Pointer auf die L�nge der Datei einstellen
				Load.read(buffer, size); //Datei einlesen
				for (unsigned int i = 0; i<size; i++)
				{
					if ((buffer[i] == char(13)) && (buffer[i + 1] == char(10))) //NL -> CRNL beim Speichern r�ckg�ngig machen
					{
						vec.push_back(10);
						i++;//index nochmal extra um eins erh�hen, da 2 Zeichen eingelesen wurden
					}
					else
					{
						vec.push_back(buffer[i]);
					}
				}
				delete[] buffer; //Speicherplatz freigeben
				Load.close();
				return true;
			}
			else
			{
				cout << "IO Thread " << "Fehler beim Laden der Datei " + filename << endl;
				Load.close();
				return false;
			}
		}

		//Speichern
		bool save(vector<string>  &vec, string filename)
		{
			unsigned int i = 0;
			ofstream Save(filename.c_str());
			while (Save.good() && (i<vec.size()));
			{
				Save << vec[i++] << endl;
			}
			Save.close();
			vec.clear();
			return true;
			//Fehler abfangen???
		}
		bool save(vector<unsigned char>&vec, string filename)
		{
			char * buffer;
			ofstream Save(filename.c_str(), ios::binary | ios::trunc);
			if (Save.good())
			{
				buffer = new char[vec.size()]; //char Pointer auf die L�nge der Datei einstellen
											   //vec nach buffer kopieren
				for (unsigned int i = 0; i<vec.size(); i++)
				{
					buffer[i] = vec[i];
				}
				Save.write(buffer, vec.size()); //Datei schreiben
				delete[] buffer; //Speicherplatz freigeben
				vec.clear();
			}
			if (Save.good())
			{
				Save.close();
				return true;
			}
			else
			{
				cout << "IO Thread " << "Fehler beim Schreiben der Datei " + filename << endl;
				Save.close();
				return false;
			}
		}
	};
};