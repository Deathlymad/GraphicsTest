#include "bmp.h"
#include <string>
#include <fstream>
#include <iostream>
#include "Def.h"

NSP_STD

//bmpread v0.1 by Linus Helfmann
//Loader nimmt an das: Standard eingehalten wurde => Felder richtig beschrieben, Farbtiefe 24Bits, Compression = 0, ansonsten gibt es Fehler aus
//1. Dateigröße bestimmen
//2. wenn Dateigröße über 54, dann Header einlesen und prüfen ob es mit BM beginnt
//3. Info lesen, falls es 40 lang ist, Compression = 0, BitCount = 24 weitermachen
//4. Width und Heigh einlesen
//5. ab Adresse 54 die Daten lesen



char* loadbmp(string path, int &width, int &height)
{
	ifstream load(path.c_str(), ios::binary | ios::ate);
	if (load.good())
	{
		//1.
		int size = (int) load.tellg(); //Größe der Datei
		load.seekg(0);
		//2.
		if(size > 54)
		{
			uint16_t typ;
			load.read(reinterpret_cast<char*>(&typ), sizeof(typ));
			bmp_header header;
			load.read(reinterpret_cast<char*>(&header), sizeof(bmp_header));
			if (typ == 19778) //= "BM"
			{
				//3.
				bmp_info info;
				load.seekg(14); //ansonsten funktioniert es nicht
				load.read(reinterpret_cast<char*>(&info), sizeof(bmp_info));
				if ((info.biBitCount == 24) && (info.biCompression == 0))
				{
					//4.
					width = info.biWidth;
					height = abs(info.biHeight);
					//5.
					//if(size == header.bfOffbits+abs(width*height*3)) testen?
					//oder andersweitig nach eof checken?
					load.seekg(header.bfOffBits);
					char * data = new char[width*height*3];

					if(info.biHeight > 0) 
					{
						load.read(data, width*height * 3);
						//Datei schließen
						load.close();
						return data;
					}
				}
			}
		}
	}
	
	// Hier sollte man nicht ankommen ;)
	cout << "bitmap Loader " << "Fehler beim lesen von " + path << endl;
	load.close();
	return nullptr;
}






