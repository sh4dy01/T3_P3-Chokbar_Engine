#include "Chokbar.h"
#include "Simulation.h"

#include "Engine/SplashScreen.h"

namespace Chokbar
{
	Simulation::Simulation()
		: Window(L"Application", NULL)
	{
	}

	Simulation::~Simulation()
	{
	}

	void Simulation::PreInitialize()
	{

		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());

		Logger::PrintDebugSeperator();

		//SplashScreen::Open();
	}


	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		}

		return Window::MessageHandler(hwnd, message, wParam, lParam);
	}
}