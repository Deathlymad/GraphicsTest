#include "Clock.h"



Clock::Clock( function<void()> init, function<void()> f, unsigned int tps) : init(init), func(f), running(false), tickRatio(1000/tps)
{
}

void Clock::run()
{
	running = true;
	t = thread([this] {update(); });
}

Clock::~Clock()
{
	running = false;
	if (t.joinable())
		t.join();
}

void Clock::update()
{
	init();
	lastSec = system_clock::now();
	while (running)
	{
		lastTick = chrono::system_clock::now();
		
		//mutex Check
		func();
		currentTicks++;

		this_thread::sleep_for(milliseconds((int)floorf(tickRatio)) - duration_cast<chrono::milliseconds>(system_clock::now() - lastTick));


		if (duration_cast<milliseconds>(system_clock::now() - lastSec) >= milliseconds(1000))
		{
			lastTicks = currentTicks;
			currentTicks = 0;
			lastSec = system_clock::now();
		}
	}
}
