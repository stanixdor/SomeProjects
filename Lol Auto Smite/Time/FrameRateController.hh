#pragma once
#include <chrono>
class FrameRateController
{
public:
	static void StartFrame();
	static void EndFrame();
	static void ChangeMaxFps(long double newFps);
	static long double GetFps();
	static long double GetDeltaTime();
private:
	static std::chrono::time_point<std::chrono::system_clock> frameStartTime;
	static long double fps;
	static long double deltaTime;
	static long double maxfps;
};

class Timer
{
public:
	long double GetTime();
	void Start();
	void Reset();
private:
	std::chrono::time_point<std::chrono::system_clock> startTime;
};
