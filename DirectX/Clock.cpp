#include "Main.h"
#include "Clock.h"

Clock::Clock() 
	:
	startTime(std::chrono::steady_clock::now())
{}

Clock Clock::GetSingleton() {
	static Clock clock;
	return clock;
}

float Clock::GetTimeSinceStart() {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count();
}
