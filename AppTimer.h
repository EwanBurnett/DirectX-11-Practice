#pragma once

class GameTimer {
public:
	GameTimer();

	//Returns time in seconds
	float GameTime() const; 
	float DeltaTime() const;

	void Reset();	//before message loop
	void Start();	//when game is unpaused
	void Stop();	//when game is paused
	void Tick();	//once per frame

	bool IsPaused();

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mStopTime;
	__int64 mPausedTime;
	__int64 mPrevTime;
	__int64 mCurrentTime;

	bool mIsStopped;
};