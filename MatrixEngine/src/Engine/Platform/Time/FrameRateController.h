// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEFrameRateController.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2007/04/26
// Purpose			:	header file for the frame rate controller
// History			:
// - 2007/04/26		:	- initial implementation
// ---------------------------------------------------------------------------
#ifndef FRAME_RATE_CONTROLLER_H
#define FRAME_RATE_CONTROLLER_H

#include "Utils/ISingleton.h"
#include "Core/DataTypes.h"


namespace FRC
{
	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	// TIME CLASS - Static interface for framerate controller
	class FrameRateController : public ISingleton<FrameRateController>
	{

	public:
		virtual bool Initialize();
		virtual void Update();

		// call between each frame for frame statistics
		void StartFrame();
		void EndFrame();
		void Reset();

		f64 GetMaxFrameRate();
		f64 GetFrameRate();
		f64 GetFrameTime();
		f64 GetFrameCounter();

		// set max frame rate
		void LockFrameRate(bool enabled){ frameRateLocked = enabled; }
		inline bool FrameRateLocked(){ return frameRateLocked; }
		void SetMaxFrameRate(f64 fps);

		// uses the CPU clock to return a time in seconds.
		static f64 GetCPUTime();

	private:

		bool frameRateLocked = true;
		f64	frameRateMax;	// clamp the frame rate to at most this number
		f64	frameRate;		// the frame rate based on the last frame
		f64	frameTime;		// time taken to process the last frame(in seconds)
		u32	frameCounter;	// number of frame since the last reset
		f64	frameTimeMin;
		f64	frameTimeStart;
		f64	frameTimeEnd;
	};


	// ----------------------------------------------------------------------------
	// ----------------------------------------------------------------------------
	// TIMER - uses Time Class for timer
	struct Timer
	{
		f64 mStartTime;
		f64 mTimeSinceLastTick;
		f32 mTimeScale;
		bool mPaused;

		// ------------------------
		// METHODS
		Timer();
		f32 Tick();		// Returns the time since last tick. Call each frame.
		void Reset();	// Resets the timer values
		void Start();	// Sets paused to false
		void Pause();	// Sets paused to true
		f32 GetTimeSinceStart(); // returns the time since the last reset
	};
}

// ---------------------------------------------------------------------------

#endif // FRAME_RATE_CONTROLLER_H

