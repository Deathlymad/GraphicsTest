#include "Clock.h"



Clock::Clock( std::function<void()> init, std::function<void()> f, unsigned int tps) : init(init), func(f), running(true), tickRatio(1000/tps)
{
	running = true;
	t = std::thread([this] {update(); });
}

void Clock::run()
{
}

Clock::~Clock()
{
	running = false;
	if (t.joinable())
		t.join();
}

void Clock::update()
{
	std::mutex m;
	m.lock();
	init();
	m.unlock();
	lastSec = system_clock::now();
	while (running)
	{
		lastTick = std::chrono::system_clock::now();

		//mutex Check
		func();
		currentTicks++;

		std::this_thread::sleep_for(milliseconds((int)std::floorf(tickRatio)) - duration_cast<std::chrono::milliseconds>(system_clock::now() - lastTick));


		if (duration_cast<milliseconds>(system_clock::now() - lastSec) >= milliseconds(1000))
		{
			lastTicks = currentTicks;
			currentTicks = 0;
			lastSec = system_clock::now();
		}
	}
}
