#include "Chokbar.h"
#include "Window.h"


namespace Win32
{
	Window::Window(WSTRING title, HICON icon, WindowType type)
		: SubObject(title, title, icon), m_Type(type)
	{
		m_Width = DEFAULT_WIDTH;
		m_Height = DEFAULT_HEIGHT;
	}

	Window::Window(WSTRING title, HICON icon, int width, int height, WindowType type)
		: SubObject(title, title, icon), m_Width(width), m_Height(height), m_Type(type)
	{
	}

	Window::~Window()
	= default;



	void Window::Initialize()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0, 0, m_Width, m_Height};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);


		m_hWnd = CreateWindowEx(0, m_Class.c_str(), m_Title.c_str(), m_Type, ((desktop.right / 2) - (m_Width / 2)), ((desktop.bottom / 2) - (m_Height / 2)), 
			m_Width, m_Height, NULL, NULL, HInstance(), NULL);

		if (m_hWnd == NULL) {
			OutputDebugString(L"Window Creation Failed!\n");
		}

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
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return SubObject::MessageHandler(hwnd, message, wParam, lParam);
	}




}