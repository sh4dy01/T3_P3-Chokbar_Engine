#pragma once

#include "Core/CoreMinimal.h"

namespace Chokbar {

	class Engine {

	public:

		Engine();
		~Engine();

		static Engine& GetInstance();

		void Initialize();
		void Run();
		void Shutdown();

	protected:

		void PreInitialize();


		void Update(float dt);


		virtual void OnResize();

	private:

		void Render();
		void CalculateFrameStats();
		bool NeedsToClose();

		float GetDeltaTime() const { return m_GameTimer.GetDeltaTime(); }


	private:

		GameTimer m_GameTimer;
		Win32::Window m_Window;

	};
}
