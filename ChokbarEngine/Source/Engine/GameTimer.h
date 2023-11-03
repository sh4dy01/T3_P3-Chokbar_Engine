#pragma once

namespace Chokbar 
{
	class GameTimer {

	public:

		GameTimer();
		~GameTimer();


		void Reset();	// Call before message loop.
		void Start();	// Call when unpaused.
		void Stop();	// Call when paused.
		void Tick();	// Call every frame.

		double GetCurrentFrameTime();

		float GetTotalTime();
		float GetGameTime() const { return m_CurrTime; }
		static double GetDeltaTime() { return m_DeltaTime; }


	private:

		double m_SecondsPerCount;
		static double m_DeltaTime;

		float m_BaseTime;
		float m_PausedTime;
		float m_StopTime;
		float m_PrevTime;
		float m_CurrTime;

		bool isStopped;

	};
}
