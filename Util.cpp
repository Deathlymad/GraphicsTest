#include "Util.h"
#include <stdio.h>
#include <fstream>
#include <iostream>

NSP_UTIL_BEG
	ThreadServer::ThreadServer(unsigned int threadCount) : _running(true), _taskList(), _threads(), _conCount(threadCount), _maxElement(0)
	{
		_threads = vector<thread*>(threadCount, new thread([this] {this->run(); }));
	}

	bool ThreadServer::hasTasks()
	{
		return !_taskList.empty();
	}

	void ThreadServer::addThreadClient(ThreadClient* client)
	{
		client->_connected = true;
		_taskList.push_back(client);
	}

	unsigned int ThreadServer::getThreadCount()
	{
		return _threads.size();
	}
	void ThreadServer::changeThreadAmt(unsigned int threadCount)
	{
		if ((threadCount - _threads.size()) > 0)
		{
			_threads.push_back(new thread([this] {run(); }));
		}//add decementing
	}

		ThreadServer::~ThreadServer()
		{
			_running = false;
			for (thread* t : _threads)
				if (t->joinable())
					t->join();
		}

	ThreadServer::ThreadServer(unsigned int threadCount, function<void()> func) : _running(true), _taskList(), _threads(), _conCount(threadCount)
	{
		_threads = vector<thread*>(threadCount, new thread(func));
	}

	void ThreadServer::run()
	{
		while (_running)
		{
			_listGuard.lock();
			if (_maxElement >= _taskList.size())
			{
				_taskList.clear();
				_maxElement = 0;
			}
			unsigned int i = _maxElement;
			_maxElement++;

			if (!_taskList.empty())
			{
				ThreadClient* curr = _taskList[i];
				curr->_func();
				curr->disconnect();
			}
			else
			{
				this_thread::sleep_for(milliseconds(50));
			}
			_listGuard.unlock();
		}
	}

	LoopedThreadServer::LoopedThreadServer(unsigned int threadCount) : ThreadServer(threadCount, [this] {_run(); }) {}

	void LoopedThreadServer::_run()
	{

		system_clock::time_point lastTick = system_clock::now();
		while (_running)
		{
			if (!_taskList.empty())
			{
				_listGuard.lock();
				if (_maxElement >= _taskList.size()) //resets
					_maxElement = 0;
				try {
					_taskList[_maxElement]->_func();
				}
				catch (bad_function_call except)
				{
					cout << except.what() << endl;
				}
				_maxElement++;
					

				_listGuard.unlock();

				this_thread::sleep_for(milliseconds(17) - duration_cast<chrono::milliseconds>(system_clock::now() - lastTick));
			}
			else
			{
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
				cout << "IO Thread " << "Fehler beim Schreiben der Datei " + filename << endl;
				Save.close();
				return false;
			}
		}
	};
};