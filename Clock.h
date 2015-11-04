#include <chrono>
#include <functional>
#include <thread>

using namespace std::chrono;

#pragma once
class Clock
{
public:
	Clock(std::function<void()>, std::function<void()>, unsigned int);

	unsigned int getLastTPS() { return lastTicks; }

	void run();

	bool isRunning() { return running; }
	void shutdown() { running = false; }

	~Clock();
private:
	//function Variable
	std::function<void()> init, func;
	//Loop Variables
	bool running;

	float tickRatio;
	unsigned short currentTicks = 0; //tps
	unsigned short lastTicks = 0;
	system_clock::time_point lastSec = std::chrono::system_clock::now();  //used to update TPS
	system_clock::time_point lastTick = std::chrono::system_clock::now(); //used to retain ticking Speed

	//Loop
	void update();

	std::thread t;
};