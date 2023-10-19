#pragma once

class GameTimer {

public:

	GameTimer();
	~GameTimer();


	void Reset();	// Call before message loop.
	void Start();	// Call when unpaused.
	void Stop();	// Call when paused.
	void Tick();	// Call every frame.

	__int64 GetCurrentFrameTime();

	float GetTotalTime();
	float GetGameTime() const { return m_CurrTime; }
	float GetDeltaTime() const { return m_DeltaTime; }


private:

	double m_SecondsPerCount;
	double m_DeltaTime;

	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrTime;

	bool isStopped;

};