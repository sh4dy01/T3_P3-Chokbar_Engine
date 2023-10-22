#pragma once

#include "Core/Core.h"

namespace Chokbar {

	class CHOKBAR_API Engine : public Win32::IApplication, public Win32::Window {

	public:

		Engine();
		~Engine();

		void PreInitialize() override;
		void Initialize() override;
		void Update(float dt) override;

		bool NeedsToClose() override;
		void Shutdown() override;

		virtual void OnResize();

		void Render();
		void CalculateFrameStats();
		void Tick();
		void ResetTimer();

		float GetDeltaTime() const { return m_GameTimer.GetDeltaTime(); }

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:

		D3DApp m_D3DApp;

		GameTimer m_GameTimer;

	};
}
