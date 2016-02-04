#include "RessourceHandler.h"

#include <fstream>


RessourceLoader::RessourceLoader() : fileLoader(4)
{
}

ThreadClient* RessourceLoader::loadCode(string file, function<void(future<char**>&)> callback)
{
	promise<char**> prom;
	ThreadClient* client = new ThreadClient([this, file, callback, &prom, &client] {
		_load(file, callback, &prom);
		client->disconnect(); });
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
