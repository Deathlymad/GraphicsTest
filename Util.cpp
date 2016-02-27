#include "Util.h"
#include <stdio.h>
#include <fstream>

#include <iostream>

#include "Log.h"

NSP_UTIL_BEG
	ThreadServer::ThreadServer(unsigned int threadCount) : _running(true), _taskList(), _maxElement(0)
	{
		unsigned temp = threadCount;
		while (temp > 0)
		{
			_taskResults.push_back(async([this] {run(); }));
			temp--;
		}
	}

	bool ThreadServer::hasTasks()
	{
		return !_taskList.empty();
	}

	void ThreadServer::addThreadClient(ThreadClient* client)
	{
		client->_connected = true;
		_listGuard.lock();
		_taskList.push_back(client);
		_listGuard.unlock();
	}

	ThreadServer::~ThreadServer()
	{
		_running = false;
		for (future<void>& fut : _taskResults)
			fut.get();
	}

	ThreadServer::ThreadServer(unsigned int threadCount, function<void()> func) : _running(true), _taskList(), _maxElement(0)
	{
		unsigned temp = threadCount;
		while (temp > 0)
		{
			_taskResults.push_back(async(func));
			temp--;
		}
	}

	void ThreadServer::run()
	{
		system_clock::time_point lastTick = system_clock::now();
		ThreadClient* temp = nullptr;
		while (_running)
		{
			_listGuard.lock();
			if (!_taskList.empty())
			{
				if (_maxElement >= _taskList.size()) //resets
					_maxElement = 0;
				temp = _taskList[_maxElement];
				if (!temp->_connected || temp->_executing)
				{
					_taskList.erase(_taskList.begin() + _maxElement);
					temp = nullptr;
					_maxElement++;
					_listGuard.unlock();
				}
				else {
					temp->_executing = true;
					_maxElement++;
					_listGuard.unlock();
					try {
						temp->_func();
						temp->disconnect();
					}
					catch (bad_function_call except)
					{
						LOG << string(except.what()) + "\n";//really bad Idea
					}
					temp->_executing = false;
					temp = nullptr;
					this_thread::sleep_for(milliseconds(50) - duration_cast<chrono::milliseconds>(system_clock::now() - lastTick));
				}
			}
			else
			{
				_listGuard.unlock();
				this_thread::sleep_for(milliseconds(50));
			}
		}
	}

	LoopedThreadServer::LoopedThreadServer(unsigned int threadCount) : ThreadServer(threadCount, [this] {_run(); }) {}

	void LoopedThreadServer::_run()
	{
		system_clock::time_point lastTick = system_clock::now();
		ThreadClient* temp = nullptr;
		while (_running)
		{
			_listGuard.lock();
			if (!_taskList.empty())
			{
				if (_maxElement >= _taskList.size()) //resets
					_maxElement = 0;
				 temp = _taskList[_maxElement];
				if (!temp->_connected || temp->_executing)
				{
					_taskList.erase(_taskList.begin() + _maxElement);
					temp = nullptr;
					_maxElement++;
					_listGuard.unlock();
				}
				else {
					temp->_executing = true;
					_maxElement++;
					_listGuard.unlock();
					try {
						temp->_func();
					}
					catch (bad_function_call except)
					{
						LOG << except.what() << "\n";//really bad Idea
					}
					temp->_executing = false;
					temp = nullptr;
					this_thread::sleep_for(milliseconds(50) - duration_cast<chrono::milliseconds>(system_clock::now() - lastTick));
				}
			}
			else
			{
				_listGuard.unlock();
				this_thread::sleep_for(milliseconds(50));
			}
		}
	}
	
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

		//Speichern
		bool save(vector<string>  &vec, string filename)
		{
			unsigned int i = 0;
			ofstream Save(filename.c_str());
			while (Save.good() && (i<vec.size()));
			{
				Save << vec[i++] << "\n";
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
				buffer = new char[vec.size()]; //char Pointer auf die Länge der Datei einstellen
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
				LOG << "IO Thread " << "Fehler beim Schreiben der Datei " + filename << "\n";
				Save.close();
				return false;
			}
		}
	};
};