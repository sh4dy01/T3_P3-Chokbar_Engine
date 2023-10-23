#include <wchar.h>
#include <WinUser.h>
#include <wtypes.h>

class InputHandler
{
public: 

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Thats all

};