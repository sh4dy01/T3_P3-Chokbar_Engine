#include "Chokbar.h"
#include "SubObject.h"

namespace Win32 {

	SubObject::SubObject(std::wstring className, std::wstring titleName, HICON icon)
		: m_Class(className), m_Title(titleName), m_hIcon(icon)
	{

	}

	SubObject::~SubObject()
	{
	}

	void SubObject::RegisterNewClass()
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);							// define the size of the window class (init)
		wcex.style = CS_HREDRAW | CS_VREDRAW;						// Redraw on Horizontal or Vertical movement/resize
		wcex.cbClsExtra = 0;										// Extra bytes to allocate following the window-class structure
		wcex.cbWndExtra = 0;										// Set to 0 because we doesn't need extra memory for now
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);		// Load the default arrow cursor
		wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(46, 46, 46));	
		wcex.hIcon = m_hIcon;
		wcex.hIconSm = m_hIcon;
		wcex.lpszClassName = m_Class.c_str();
		wcex.lpszMenuName = nullptr;
		wcex.hInstance = HInstance();								// Set the instance that we have created
		wcex.lpfnWndProc = SetupMessageHandler;							// Set the default window procedure for our window class

		RegisterClassEx(&wcex);
	}

	LRESULT SubObject::SetupMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			auto* const pWnd = static_cast<SubObject*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&SubObject::AssignMessageHandler));
			return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT SubObject::AssignMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto* const pWnd = reinterpret_cast<SubObject*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->MessageHandler(hWnd, msg, wParam, lParam);
	}

	LRESULT SubObject::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

}

