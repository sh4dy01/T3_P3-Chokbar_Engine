#include "Chokbar.h"
#include "TimeManager.h"


float TimeManager::m_DeltaTime = 0.0f;
float TimeManager::m_CurrTime = 0.0f;
float TimeManager::m_TimeScale = 1.0f;
float TimeManager::m_FixedTime = 0.02f;


TimeManager::TimeManager()
	: m_BaseTime(0), m_PausedTime(0), m_StopTime(0), m_PrevTime(0), isStopped(false)
{
	LARGE_INTEGER perfCountFreq;
	memset(&perfCountFreq, 0, sizeof(perfCountFreq));
	QueryPerformanceFrequency(&perfCountFreq);
	m_freq = perfCountFreq.QuadPart;

	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	m_performtime = currTime.QuadPart;
}

TimeManager::~TimeManager()
= default;

void TimeManager::Reset()
{
	const auto currTime = GetCurrentFrameTime();

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	isStopped = false;
}

void TimeManager::Start()
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

void TimeManager::Stop()
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

void TimeManager::Tick()
{
	if (isStopped)
	{
		m_DeltaTime = 0.0;	// We want to return 0 to tell that the time's not ticking anymore
		return;
	}

	// Get the time this frame
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);
	m_CurrTime = (currTime.QuadPart-m_performtime)/m_freq;

	// Time difference between this frame and the previous.
	m_DeltaTime = (m_CurrTime - m_PrevTime);

	// Prepare for next frame.
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0f)
	{
		m_DeltaTime = 0.0f;
	}
}

__int64 TimeManager::GetCurrentFrameTime()
{
	__int64 currTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currTime));

	return currTime;
}

float TimeManager::GetTotalTime()
{
	if (isStopped)
	{
		return static_cast<float>(((m_StopTime - m_PausedTime) - m_BaseTime) / m_freq);
	}
	else
	{
		return static_cast<float>(((m_CurrTime - m_PausedTime) - m_BaseTime) / m_freq);
	}
}

void TimeManager::SetTimeScale(float timeScale)
{
	m_TimeScale = timeScale <= 0.0f ? 0 : timeScale;
}

void TimeManager::SetFixedTime(float fixedTime)
{
	m_FixedTime = fixedTime <= 0.0f ? 0 : fixedTime;
}
