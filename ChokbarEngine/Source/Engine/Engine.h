#pragma once

#include "Core/Core.h"


namespace Chokbar {

	class Engine {

		Engine();
		~Engine();

	public:

		static Engine* GetInstance();
		static Coordinator* GetCoordinator();
		static InputHandler* GetInput();
		static Camera* GetMainCamera();

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

		static Engine* m_Instance;

		GameTimer m_GameTimer;
		Coordinator m_Coordinator;
		CameraManager m_CameraManager;

		//physic
		Win32::Window m_Window;

		InputHandler m_InputHandler;

	};
}
