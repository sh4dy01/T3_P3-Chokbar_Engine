#include "Chokbar.h"
#include "SplashScreen.h"

#include "Platform/Windows/Win32Utils.h"


namespace SplashScreen
{
	#define WM_OUTPUTMESSAGE (WM_USER + 1)

	SplashWindow* m_SplashWindow;

	void Open()
	{
		if (m_SplashWindow != nullptr)
			return;

		m_SplashWindow = new SplashWindow();
	}

	void Close()
	{
		
	}

	void SetMessage(const WCHAR* message)
	{
		PostMessage(m_SplashWindow->Handle(), WM_OUTPUTMESSAGE, (WPARAM)message, 0);
	}


}

SplashWindow::SplashWindow()
{
	wcscpy_s(m_OutputMessage, L"Starting Splash Screen...");

	CreateNewWindow(500, 300, L"Splash", nullptr, Win32::WindowType::POPUP);
}

SplashWindow::~SplashWindow()
= default;

LRESULT SplashWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		HBITMAP hbitmap;
		HDC hdc, hmemdc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hwnd, &ps);

		//Win32::Utils::AddBitmap(L"..\\ChokbarEngine\\Content\\Images\\ChokbarSplashScreen.bmp", hdc);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetTextAlign(hdc, TA_CENTER);

		TextOut(hdc, m_Width / 2, m_Height - 30, m_OutputMessage, wcslen(m_OutputMessage));
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_OUTPUTMESSAGE:
		{
			WCHAR* message = (WCHAR*)wParam;

			wcscpy_s(m_OutputMessage, message);
			RedrawWindow(Handle(), NULL, NULL, RDW_INVALIDATE);

			return 0;
		}
	default:
		break;
	}

	return MessageHandler(hwnd, message, wParam, lParam);
}
