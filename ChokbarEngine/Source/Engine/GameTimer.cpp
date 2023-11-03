#include "Chokbar.h"
#include "GameTimer.h"


float GameTimer::m_DeltaTime = -1.0f;

GameTimer::GameTimer()
	: m_SecondsPerCount(0.0), m_BaseTime(0), m_PausedTime(0), m_StopTime(0), m_PrevTime(0), m_CurrTime(0), isStopped(false)
{
	__int64 countsPerSec = 0;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));

	m_SecondsPerCount = 1.0 / static_cast<double>(countsPerSec);

}

GameTimer::~GameTimer()
= default;

void GameTimer::Reset()
{
	const auto currTime = GetCurrentFrameTime();

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	isStopped = false;
}

void GameTimer::Start()
{
	const auto startTime = GetCurrentFrameTime();

	// Accumulate the time elapsed between stop and start pairs.
	//
	// |<-------d------->|
	// ---------------*-----------------*------------> time
	// mStopTime startTime
	// If we are resuming the timer from a stopped state...
	if (isStopped)
	{
		// then accumulate the paused time.
		m_PausedTime += (startTime - m_StopTime);

		// since we are starting the timer back up, the current
		// previous time is not valid, as it occurred while paused.
		// So reset it to the current time.
		m_PrevTime = startTime;

		// no longer stopped...
		m_StopTime = 0;
		isStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!isStopped)
	{
		const auto currTime = GetCurrentFrameTime();

		// Otherwise, save the time we stopped at, and set
		// the Boolean flag indicating the timer is stopped.
		m_StopTime = currTime;
		isStopped = true;
	}
}

void GameTimer::Tick()
{
	if (isStopped)
	{
		m_DeltaTime = 0.0;	// We want to return 0 to tell that the time's not ticking anymore
		return;
	}

	// Get the time this frame
	__int64 currTime = 0;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));
	m_CurrTime = currTime;

	// Time difference between this frame and the previous.
	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;

	// Prepare for next frame.
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}
}

float GameTimer::GetCurrentFrameTime()
{
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

	return currTime;
}

float GameTimer::GetTotalTime()
{
	// If we are stopped, do not count the time that has passed
	// since we stopped. Moreover, if we previously already had
	// a pause, the distance mStopTime - mBaseTime includes paused
	// time,which we do not want to count. To correct this, we can
	// subtract the paused time from mStopTime:
	//
	// previous paused time
	// |<----------->|
	// ---*------------*-------------*-------*-----------*------> time
	// mBaseTime mStopTime mCurrTime
	if (isStopped)
	{
		return static_cast<float>(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count. To correct this, we can subtract
	// the paused time from mCurrTime:
	//
	// (mCurrTime - mPausedTime) - mBaseTime
	//
	// |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	// mBaseTime mStopTime startTime mCurrTime
	else
	{
		//OutputDebugString(L"Total time: ");
		//OutputDebugStringA(std::to_string(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount).c_str());
		//OutputDebugString(L"\n");

		return static_cast<float>(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
}
