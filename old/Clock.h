#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Def.h"

NSP_STD

using namespace chrono;

#pragma once
class Clock //deprecated, can be removed once dynamic tick timing is added to thread manager
{
public:
	Clock(function<void()>&, function<void()>&, unsigned int);

	unsigned int getLastTPS() { return lastTicks; }

	void run();
	

	bool isRunning() { return running; }
	void shutdown() { running = false; }

	~Clock();
private:
	//function Variable
	function<void()> init, func;
	//Loop Variables
	bool running;

	float tickRatio;
	unsigned short currentTicks = 0; //tps
	unsigned short lastTicks = 0;
	system_clock::time_point lastSec = chrono::system_clock::now();  //used to update TPS
	system_clock::time_point lastTick = chrono::system_clock::now(); //used to retain ticking Speed

	//Loop
	void update();

	thread t;
};