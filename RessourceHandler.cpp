#include "RessourceHandler.h"

#include <fstream>


RessourceLoader::RessourceLoader() : fileLoader(4)
{
}

ThreadClient* RessourceLoader::loadFile(string file, function<void(future<char**>&)> callback)
{
	promise<char**> prom;
	ThreadClient* client = new ThreadClient([this, file, callback, &prom] {
		_load(file, callback, &prom);
	});
	fileLoader.addThreadClient(client);
	return client;
}
ThreadClient* RessourceLoader::loadFile(string file, function<void(ifstream&)> loader)
{
	ThreadClient* client = new ThreadClient([this, file, loader, &client] {
		ifstream f(file, ios::binary);
		if (f.is_open())
			loader(f);
	});
	fileLoader.addThreadClient(client);
	return client;
}
ThreadClient* RessourceLoader::loadFile(string file, function<void(vector<string>)> loader)
{
	ThreadClient* client = new ThreadClient([this, file, loader, &client] {
		ifstream f(file, ios::binary);
		if (f.is_open())
		{
			vector<string> temp;
			string Line = "";
			while (getline(f, Line))
			{
				temp.push_back(Line);
			}
			f.close();
			loader(temp);
		}
	});
	fileLoader.addThreadClient(client);
	return client;
}

RessourceLoader::~RessourceLoader()
{
}

void RessourceLoader::_load(string file, function<void(future<char**>&)> callback, promise<char**>* prom)
{
	string fileCode;
	ifstream Fstream(file);
	//Read Code from File
	if (Fstream.is_open())
	{
		string Line = "";
		while (getline(Fstream, Line))
		{
			fileCode += Line + "\n";
		}
		Fstream.close();
		char* charArr = const_cast<char*>(fileCode.c_str());
		prom->set_value(&charArr);
	}
	else
		prom->set_value(nullptr);
	callback(prom->get_future());
}
