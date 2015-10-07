//iofunctions v0.1 by Linus Helfmann
//Sonderzeichen (�,�,�,�) funktionieren nur wenn Datei in ANSI und OEM 850 kodiert
//(Kodierung -> Zeichensatz -> Westeurop�isch -> OEM 850 in Notepadd++) 

#include <vector>
#include <string>

namespace io
{

	//Hilfsfunktionen (verschieben?) ja, util
	bool strsep(std::vector<std::string> &vec, std::string &s);
	bool strsep(std::vector<std::string> &vec, std::string &s, char c);

	//Laden
	bool load(std::vector<std::string>  &vec, std::string filename);
	bool load(std::vector<unsigned char>&vec, std::string filename);

	//Speichern
	bool save(std::vector<std::string>  &vec, std::string filename);
	bool save(std::vector<unsigned char>&vec, std::string filename);

};
