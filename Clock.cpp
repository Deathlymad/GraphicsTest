#include "Clock.h"



Clock::Clock( std::function<void()> init, std::function<void()> f) : init(init), func(f), running(true)
{}

void Clock::run()
{
	running = true;
	t = std::thread([this] {update(); });
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
		lastTick = std::chrono::system_clock::now();

		
		func();
		currentTicks++;

		std::this_thread::sleep_for(milliseconds((int)std::floorf(16.666f)) - duration_cast<std::chrono::milliseconds>(system_clock::now() - lastTick));


		if (duration_cast<milliseconds>(system_clock::now() - lastSec) >= milliseconds(1000))
		{
			lastTicks = currentTicks;
			currentTicks = 0;
			lastSec = system_clock::now();
		}
	}
}
