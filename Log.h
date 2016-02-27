#ifndef LOG
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

class Log
{
public:
	Log() : LogFile("Log.txt") { LogFile.clear(); }

	~Log() { LogFile.close(); }

	Log& operator<< (const char *c)
	{
		std::lock_guard<std::mutex> t(LogProt);
		std::cout << c;
		LogFile << c;
		LogFile.flush();
		return *this;
	}
	Log& operator<< (std::string s)
	{
		*this << s.c_str();
		return *this;
	}

	static Log* Log::ConsoleLogger;
private:
	std::ofstream LogFile;
	std::mutex LogProt;
};

#define LOG *Log::ConsoleLogger
#endif