#include "KnightWin.h"
#include "Timer.h"

GameTimer::GameTimer()
	: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mCurrentTime(0),
	mPausedTime(0), mPrevTime(0), mStopTime(0), mIsStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

float GameTimer::GameTime() const
{
	if (mIsStopped) {
		return((float)((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
	else {
		return((float)((mCurrentTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}

}

float GameTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mIsStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime = 0;

	//Starting from a pause
	if (mIsStopped) {
		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;

		mStopTime = 0;
		mIsStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!mIsStopped) {
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		//Save the time we paused at, and pause.
		mPausedTime = currTime;
		mIsStopped = true;
	}
}

void GameTimer::Tick()
{
	//If the application is paused, do nothing.
	if (mIsStopped) {
		mDeltaTime = 0.0;
		return;
	}


	//Get the time this frame
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrentTime = currTime;


	//Calculate the time difference between this frame and the last
	mDeltaTime = (mCurrentTime - mPrevTime) * mSecondsPerCount;

	//Prepare for the next frame
	mPrevTime = mCurrentTime;

	//Force non-negative deltatime
	if (mDeltaTime < 0) {
		mDeltaTime = 0.0;
	}
	
}

bool GameTimer::IsPaused()
{
	return mIsStopped;
}

