#include "Chokbar.h"
#include "Window.h"


namespace Win32
{
	Window::Window()
		: SubObject(L"Default", L"Default", nullptr), m_Type(RESIZABLE)
	{
		m_Width = DEFAULT_WIDTH;
		m_Height = DEFAULT_HEIGHT;
	}

	Window::~Window()
	= default;


	void Window::CreateNewWindow(int width, int height, const WSTRING& title, HICON icon, WindowType type)
	{
		m_Type = type;
		m_Title = title;
		m_Class = title;
		m_hIcon = icon;

		m_Width = width;
		m_Height = height;

		RegisterNewClass();

		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0, 0, m_Width, m_Height };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

		m_hWnd = CreateWindow(m_Class.c_str(), m_Title.c_str(),
			m_Type, ((desktop.right / 2) - (m_Width / 2)), ((desktop.bottom / 2) - (m_Height / 2)), m_Width, m_Height, nullptr, nullptr, HInstance(), (void*)this);

		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
	}

	void Window::PollEvent()
	{
		MSG msg;
		while (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	LRESULT Window::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{

		switch (message) 
		{
		case WM_QUIT:
			break;
		case WM_CLOSE:
			needsToClose = true;
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			break;
		}

		return SubObject::MessageHandler(hwnd, message, wParam, lParam);
	}

	void Window::SetNewSize(int newWidth, int newHeight)
	{
		m_Width = newWidth;
		m_Height = newHeight;
	}

}