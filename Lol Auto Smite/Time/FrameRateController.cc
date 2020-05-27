#include "FrameRateController.hh"

std::chrono::time_point<std::chrono::system_clock> FrameRateController::frameStartTime;
long double FrameRateController::fps = 0.;
long double FrameRateController::deltaTime = 0.;
long double FrameRateController::maxfps = 300.;

void FrameRateController::StartFrame()
{
	FrameRateController::frameStartTime = std::chrono::system_clock::now();
}

void FrameRateController::EndFrame()
{
	do
	{
		auto end = std::chrono::system_clock::now();
		FrameRateController::deltaTime = static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - FrameRateController::frameStartTime).count());
		FrameRateController::fps = 1000000000. / FrameRateController::deltaTime;
	} while (FrameRateController::fps > FrameRateController::maxfps);
}

void FrameRateController::ChangeMaxFps(long double newFps)
{
	FrameRateController::maxfps = newFps;
}

long double FrameRateController::GetFps()
{
	return FrameRateController::fps;
}

long double FrameRateController::GetDeltaTime()
{
	return FrameRateController::deltaTime / 1000000000.;
}

long double Timer::GetTime()
{
	auto end = std::chrono::system_clock::now();
	long double timeInNanoS =  static_cast<long double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - startTime).count());
	long double timeInSeconds = 1000000000. / timeInNanoS;
	return timeInSeconds;
}

void Timer::Start()
{
	startTime = std::chrono::system_clock::now();
}

void Timer::Reset()
{
	Start();
}
