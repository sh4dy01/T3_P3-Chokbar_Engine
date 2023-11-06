#include "Chokbar.h"
#include "SplashScreen.h"

#include "Platform/Windows/Win32Utils.h"

/*
namespace SplashScreen
{
	#define WM_OUTPUTMESSAGE (WM_USER + 1)

	SplashWindow* m_SplashWindow;

	void Open()
	{
		if (m_SplashWindow != nullptr)
			return;

		m_SplashWindow = NEW SplashWindow();
	}

	void Close()
	{
		
	}

	void SetMessage(const WCHAR* message)
	{
		PostMessage(m_SplashWindow->GetHandle(), WM_OUTPUTMESSAGE, (WPARAM)message, 0);
	}


}

SplashWindow::SplashWindow()
{
	wcscpy_s(m_OutputMessage, L"Starting Splash Screen...");

	CreateNewWindow(500, 300, L"Splash", nullptr, Win32::WindowType::POPUP);
}

SplashWindow::~SplashWindow()
= default;
*/