#pragma once

#include "Core/CoreMinimal.h"
#include "Engine/GameTimer.h"

namespace Chokbar {

	class CHOKBAR_API Engine : public Win32::Window {

	public:

		Engine();
		~Engine();

		static Engine& GetInstance() { static Engine m_Instance; return m_Instance; }

		void Initialize() override;
		bool NeedsToClose();

		virtual void OnResize();

		void Update(const float dt);
		void Run();
		void Shutdown();


		float GetDeltaTime() const { return m_GameTimer.GetDeltaTime(); }


	protected:

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:

		GameTimer m_GameTimer;

		void PreInitialize();
		void Render();
		void CalculateFrameStats();
		void Tick();
		void ResetTimer();


	};
}
