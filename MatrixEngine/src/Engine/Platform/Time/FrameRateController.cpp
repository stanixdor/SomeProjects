// ---------------------------------------------------------------------------
// Project Name		:	FrameRateController
// File Name		:	FrameRateController.cpp
// Author			:	Joaquin Bilbao
// Purpose			:	implementation of the frame rate controller
// History			:
// -11/10/2017		:	- Took the frame rate controller of the alpha engine and
//						suited it for out engine
// ---------------------------------------------------------------------------
#include "FrameRateController.h"
#include <Windows.h> //QueryPerformance... functions.

// ---------------------------------------------------------------------------
// Defines

#define FRAME_RATE_SYNC_TO_RETRACE 0
namespace FRC
{
	// ---------------------------------------------------------------------------
	// get the current time in seconds

	f64 FrameRateController::GetCPUTime()
	{
		s64 f, t;
		f64 r, r0, r1;

		QueryPerformanceFrequency((LARGE_INTEGER*)(&f));
		QueryPerformanceCounter((LARGE_INTEGER*)(&t));

		//@FIXED - precision warning
		r0 = f64(t / f);
		r1 = (t - ((t / f) * f)) / (f64)(f);
		r = r0 + r1;

		return r;//r0 + r1;
	}

	// ---------------------------------------------------------------------------
	// Functions implementations

	//FrameRateController::FrameRateController()
	//{}

	bool FrameRateController::Initialize()
	{
		FrameRateController::frameCounter = 0;
		FrameRateController::frameRateMax = 60;
		FrameRateController::frameRate = FrameRateController::frameRateMax;
		FrameRateController::frameTime = 1.0 / FrameRateController::frameRate;
		FrameRateController::frameTimeMin = 1.0 / FrameRateController::frameRateMax;
		return true;
	}

	// ---------------------------------------------------------------------------

	void FrameRateController::Reset()
	{
		//AE_ASSERT_MESG(gAEFrameRateMax > 0.0, "maximum frame rate MUST be greater than 0");

		FrameRateController::frameCounter = 0;
		FrameRateController::frameRate = frameRateMax;
		FrameRateController::frameTime = 1.0 / FrameRateController::frameRate;
		FrameRateController::frameTimeMin = 1.0 / FrameRateController::frameRateMax;
	}
	void FrameRateController::Update()
	{ 
		if (frameCounter)
			EndFrame();
		else
			frameCounter++;
		StartFrame(); 
	}

	// ---------------------------------------------------------------------------

	void FrameRateController::StartFrame()
	{
		FrameRateController::frameTimeStart = FrameRateController::GetCPUTime();
	}

	// ---------------------------------------------------------------------------

	void FrameRateController::EndFrame()
	{

		// if the total time spent is less than the minimum required time to 
		// maintain the maximum frame rate, wait
		do
		{
			FrameRateController::frameTimeEnd = GetCPUTime();
			// calculate the amount of time spend this frame
			frameTime = frameTimeEnd - frameTimeStart;
		}
		while (frameRateLocked && (frameTime) < FrameRateController::frameTimeMin);


		//@FIXED - Reset the frame rate variable
		frameRate = 1.0 / frameTime;

		// increment the total number of counter
		frameCounter++;
	}

	f64 FrameRateController::GetMaxFrameRate()
	{
		return frameRateMax;
	}
	f64 FrameRateController::GetFrameRate()
	{
		return frameRate;
	}
	f64 FrameRateController::GetFrameTime()
	{
		return frameTime;
	}
	f64 FrameRateController::GetFrameCounter()
	{
		return frameCounter;
	}

	// Setters
	void FrameRateController::SetMaxFrameRate(f64 fps)
	{
		frameRateMax = fps;
		FrameRateController::frameTimeMin = 1.0 / FrameRateController::frameRateMax;
	}
	// ---------------------------------------------------------------------------
	// Static functions implementations

	// ---------------------------------------------------------------------------

	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	// Timer
	Timer::Timer()
		: mStartTime(0.0)
		, mTimeSinceLastTick(0.0)
		, mTimeScale(1.0f)
		, mPaused(false)
	{
		Reset();
	}
	f32 Timer::Tick()
	{
		// if the timer is paused, return 0.0 delta time
		if (mPaused)
		{
			// Update the time stamp to avoid having 
			// a big delta time when the game isn't paused
			mTimeSinceLastTick = FrameRateController::GetCPUTime();
			return 0.0f;
		}

		f64 dt = FrameRateController::GetCPUTime() - mTimeSinceLastTick;
		mTimeSinceLastTick = FrameRateController::GetCPUTime();
		return static_cast<f32>(dt)* mTimeScale;
	}
	void Timer::Reset()
	{
		// reset the time stamps to the current time
		mStartTime =
			mTimeSinceLastTick = FrameRateController::GetCPUTime();
	}
	void Timer::Start()
	{
		mPaused = false;
	}
	void Timer::Pause()
	{
		mPaused = true;
	}
	f32 Timer::GetTimeSinceStart()
	{
		f64 total_time = FrameRateController::GetCPUTime() - mStartTime;
		return static_cast<f32>(total_time)*  static_cast<f32>(mTimeScale);
	}
}