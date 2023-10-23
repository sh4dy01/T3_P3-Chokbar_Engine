#include "InputHandler.h"

LRESULT CALLBACK InputHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 'A':
			MessageBox(NULL, L"A", L"Key Pressed", MB_OK);
			break;
		case 'D':
			MessageBox(NULL, L"D", L"Key Pressed", MB_OK);
			break;
		case VK_SHIFT :
			MessageBox(NULL, L"Shift", L"Key Pressed", MB_OK); //Bonus
			break;
		case VK_INSERT:
			MessageBox(NULL, L"Insert", L"Key Pressed", MB_OK); //Change Sound
			break;
		}
	}
	break;

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}