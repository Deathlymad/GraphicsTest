#include "iofunctions.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
//TODO Move
bool io::strsep(std::vector<std::string> &vec, std::string &s)
{
	unsigned int begin=0,end=0;
	vec.clear();
	while(begin<s.length())
	{
		end = begin+(s.substr(begin)).find_first_of(' ');
		if((end>=begin)&&(end!=0))
		{
			vec.push_back(s.substr(begin,end-begin));
		}
		else
		{
			vec.push_back(s.substr(begin));
			break;
		}
		begin = end+1;
	}
	return true;
}

bool io::strsep(std::vector<std::string> &vec, std::string &s, char c)
{
	unsigned int begin=0;
	unsigned int end=0;
	vec.clear();
	while(begin<s.size())
	{
		if (s.substr(begin).find_first_of(c) <= s.substr(begin).size())
			end = begin+(s.substr(begin).find_first_of(c));
		if((end>=begin)&&(end!=0))
		{
			vec.push_back(s.substr(begin,end-begin));
		}
		else
		{
			vec.push_back(s.substr(begin));
			break;
		}
		begin = end+1;
	}
	return true;
}

bool io::load(std::vector<std::string> &vec, std::string filename) //läd eine Text-datei in ein dynamischen Stringarray => vielseitig nutzbar
{
	std::string temp;
	vec.clear();
	std::ifstream Text(filename.c_str());

	if (!Text)
		Text.open( filename.c_str(), std::ios::in);
	//File doesn't exists
    while(Text.good())
    {
        getline(Text, temp);
		if((temp.length() > 1)&&(temp[0] == '#')&&(temp[1] == '#')) //Dateiende
		{
			break;
		}
		if((temp.length() > 1)&&(temp[0] != '#')&&!((temp.length() <= 1)&&!isalnum(temp[0]))) 
		{						//wenn kein Kommentar oder Leerzeile (Kommentare merken? sie werden später vielleicht gebraucht um Bereiche zu erkennen)				
			vec.push_back(temp);//oder man macht eine Zeile wo ein Schlüsselwort steht z.b. "Eigenschaften" oder man macht schreibt immer "break"???
		}						//Die Zeilen können noch am Ende ein Kommentar enthalten!!!
	}
	Text.close();
	
	vec.resize(vec.size()); //überflüssig bereitgestellter Speicher wird gelöscht

	if((vec.size() == 0))
	{
		std::cout << "IO Handler " << "Fehler beim Laden der Datei " + filename << std::endl;
		return false;
	}
	return true;
}

bool io::save(std::vector<std::string> &vec, std::string filename)
{
	unsigned int i=0;
	std::ofstream Save(filename.c_str());
	while(Save.good()&&(i<vec.size()));
	{
		Save << vec[i++] << std::endl;
	}
	Save.close();
	vec.clear();
	return true;
	//Fehler abfangen???
}

bool io::load(std::vector<unsigned char> &vec, std::string filename)
{
	unsigned int size;
	char * buffer;
	vec.clear();
	std::ifstream Load(filename.c_str(), std::ios::binary|std::ios::ate); //http://www.cplusplus.com/doc/tutorial/files/
	if(Load.good())
	{
		size = (unsigned int) Load.tellg(); //ate => Ende der Datei, tellg gibt die Position zurück und damit die Länge der Datei!
							 //Dateien immer in einem Stück lesen? oder irgentwann (z.b. ab ein paar KB) zerstückeln (mit Threads)
							 //das währe beim Laden und Verarbeiten von großen Dateien hilfreich, z.b. bei Objekten
		Load.seekg(0, std::ios::beg); //position wird wieder auf Anfang gesetzt
		buffer = new char [size]; //char Pointer auf die Länge der Datei einstellen
		Load.read(buffer,size); //Datei einlesen
		for(unsigned int i=0;i<size;i++)
		{
			if((buffer[i]==char(13))&&(buffer[i+1]==char(10))) //NL -> CRNL beim Speichern rückgängig machen
			{
				vec.push_back(10);
				i++;//index nochmal extra um eins erhöhen, da 2 Zeichen eingelesen wurden
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
		std::cout << "IO Thread " << "Fehler beim Laden der Datei " + filename << std::endl;
		Load.close();
		return false;
	}
}

bool io::save(std::vector<unsigned char>	&vec, std::string filename)
{
	char * buffer;
	std::ofstream Save(filename.c_str(), std::ios::binary|std::ios::trunc); //http://www.cplusplus.com/doc/tutorial/files/
	if(Save.good())
	{
		buffer = new char [vec.size()]; //char Pointer auf die Länge der Datei einstellen
		//vec nach buffer kopieren
		for(unsigned int i=0;i<vec.size();i++)
		{
			buffer[i] = vec[i];
		}
		Save.write(buffer,vec.size()); //Datei schreiben
		delete[] buffer; //Speicherplatz freigeben
		vec.clear();
	}
	if(Save.good())
	{
		Save.close();
		return true;
	}
	else
	{
		std::cout << "IO Thread " << "Fehler beim Schreiben der Datei " + filename << std::endl;
		Save.close();
		return false;
	}
}
