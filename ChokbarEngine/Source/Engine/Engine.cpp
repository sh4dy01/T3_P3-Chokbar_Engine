#include "Chokbar.h"
#include "Engine.h"
#include "Core/Core.h"

namespace Chokbar
{
	Engine::Engine()
	= default;

	Engine::~Engine()
	= default;

	Engine& Engine::GetInstance()
	{
		static Engine engine;
		return engine;
	}

#pragma region INIT

	void Engine::PreInitialize()
	{
		/*
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());

		Logger::PrintDebugSeperator();
		*/

		//SplashScreen::Open();
	}

	void Engine::Initialize()
	{
		PreInitialize();

		m_Window.CreateNewWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, PerGameSettings::GameName(), PerGameSettings::MainIcon(), Win32::RESIZABLE);

		D3DApp::GetInstance()->InitializeD3D12(&m_Window);

		
	}

#pragma endregion

#pragma region MAIN

	void Engine::Run()
	{
		m_GameTimer.Reset();

		while (!NeedsToClose())
		{
			m_Window.PollEvent();

			m_GameTimer.Tick();

			Update(GetDeltaTime());
			Render();
		}
	}

	bool Engine::NeedsToClose()
	{
		return m_Window.NeedsToClose();
	}

	void Engine::Update(float dt)
	{
		m_InputHandler.CheckInput();
		D3DApp::GetInstance()->Update(dt, m_GameTimer.GetTotalTime());
		CalculateFrameStats();
	}

	void Engine::Render()
	{
		D3DApp::GetInstance()->Render();
	}

#pragma endregion



	void Engine::CalculateFrameStats()
	{
		// Code computes the average frames per second, and also the
		// average time it takes to render one frame.  These stats
		// are appended to the window caption bar.

		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		// Compute averages over one second period.
		if ((m_GameTimer.GetTotalTime() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;

			std::wstring fpsStr = std::to_wstring(fps);
			std::wstring mspfStr = std::to_wstring(mspf);

			std::wstring windowText = L"    fps: " + fpsStr + L"   mspf: " + mspfStr;

			SetWindowText(m_Window.GetHandle(), windowText.c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

	void Engine::OnResize()
	{
		D3DApp::GetInstance()->OnResize(m_Window.GetWidth(), m_Window.GetHeight());
	}

	void Engine::Shutdown()
	{

	}


}
