#ifndef H_CLOCK
#define H_CLOCK
class Clock {
public:
	static Clock GetSingleton();

	float GetTimeSinceStart();
private:
	Clock();

	std::chrono::steady_clock::time_point startTime;
};
#endif