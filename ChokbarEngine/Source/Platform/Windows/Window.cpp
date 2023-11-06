#include "Chokbar.h"
#include "Window.h"


namespace Win32
{
	Window::Window()
		: m_Width(DEFAULT_WIDTH), m_Height(DEFAULT_HEIGHT), m_Type(RESIZABLE), m_Hwnd(nullptr), m_Title(L"ChokbarEngine"), m_hIcon(nullptr)
	{
	}

	Window::~Window() = default;

	void Window::CreateNewWindow(int width, int height, const WSTRING& title, HICON icon, WindowType type)
	{
		m_Type = type;
		m_Title = title;
		m_hIcon = icon;

		m_Width = width;
		m_Height = height;

		RegisterNewClass();

		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT R = { 0, 0, m_Width, m_Height };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

		m_Hwnd = CreateWindow(m_Title.c_str(), m_Title.c_str(),
			m_Type, ((desktop.right / 2) - (m_Width / 2)), ((desktop.bottom / 2) - (m_Height / 2)), m_Width, m_Height, nullptr, nullptr, HInstance(), (void*)this);

		if (m_Hwnd == NULL)
		{
			OutputDebugString(L"Window Creation Failed!\n");
			assert(false);
		}

		SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		ShowWindow(m_Hwnd, SW_SHOW);
		UpdateWindow(m_Hwnd);
	}

	void Window::PollEvent()
	{
		MSG msg;
		while (PeekMessage(&msg, m_Hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::RegisterNewClass()
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);			   // define the size of the window class (init)
		wcex.style = CS_HREDRAW | CS_VREDRAW;		   // Redraw on Horizontal or Vertical movement/resize
		wcex.cbClsExtra = 0;						   // Extra bytes to allocate following the window-class structure
		wcex.cbWndExtra = 0;						   // Set to 0 because we doesn't need extra memory for now
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); // Load the default arrow cursor
		wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(46, 46, 46));
		wcex.hIcon = m_hIcon;
		wcex.hIconSm = m_hIcon;
		wcex.lpszClassName = m_Title.c_str();
		wcex.lpszMenuName = nullptr;
		wcex.hInstance = HInstance(); // Set the instance that we have created
		wcex.lpfnWndProc = WindowProcedure;

		RegisterClassEx(&wcex);
	}

	LRESULT CALLBACK Window::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		switch (message)
		{
		case WM_QUIT:
			break;

		case WM_SETFOCUS:
			Engine::GetInstance()->OnApplicationFocus();
			DEBUG_LOG("Focus gained");
			break;

		case WM_KILLFOCUS:
			Engine::GetInstance()->OnApplicationLostFocus();
			DEBUG_LOG("Focus lost");

			break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE) {
				Engine::GetInstance()->OnApplicationLostFocus();
				DEBUG_LOG("Focus lost after escape");
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (GetFocus() != hwnd) {
				SetFocus(hwnd);
				Engine::GetInstance()->OnApplicationFocus();
				DEBUG_LOG("Focus set to window after mouse click.");
			}
			break;


		case WM_CLOSE:
			window->needsToClose = true;
			PostQuitMessage(0);
			break;
		case WM_DESTROY:
			break;
		}

		return DefWindowProcW(hwnd, message, wParam, lParam);
	}

	void Window::SetNewSize(int newWidth, int newHeight)
	{
		m_Width = newWidth;
		m_Height = newHeight;
	}
}