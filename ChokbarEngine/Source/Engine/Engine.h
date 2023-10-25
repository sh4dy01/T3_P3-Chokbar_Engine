#pragma once

#include "Core/CoreMinimal.h"

#include "Engine/ECS/Base/Coordinator.h"
#include "GameTimer.h"
#include "Platform/Windows/Window.h"


namespace Chokbar {

	class Engine {

	public:

		Engine();
		~Engine();

		static Engine& GetInstance();
		static Coordinator& GetCoordinator();

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

	private:

		GameTimer m_GameTimer;
		Coordinator m_Coordinator;

		//physic
		Win32::Window m_Window;

	};
}
