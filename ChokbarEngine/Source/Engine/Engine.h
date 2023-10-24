#pragma once

#include "Core/CoreMinimal.h"

namespace Chokbar {

	class Engine {

	public:

		Engine();
		~Engine();

	protected:

		void PreInitialize() override;
		void Initialize() override;
		void Run() override;
		void Shutdown() override;

		void Update(float dt) override;


		virtual void OnResize();

	private:

		void Render();
		void CalculateFrameStats();
		bool NeedsToClose() override;

		float GetDeltaTime() const { return m_GameTimer.GetDeltaTime(); }


	protected:

		LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	private:

		GameTimer m_GameTimer;

	};
}
