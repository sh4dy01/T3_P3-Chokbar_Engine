#pragma once

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600


namespace Win32 {

    enum WindowType : DWORD {
        STATIC = WS_OVERLAPPED,
        RESIZABLE = WS_OVERLAPPEDWINDOW,
        POPUP = WS_POPUP
    };

    namespace Utils {

        BOOL CHOKBAR_API AddBitmap(CONST WCHAR* szFileName, HDC hWinDC, INT x = 0, INT y = 0);

    }
}