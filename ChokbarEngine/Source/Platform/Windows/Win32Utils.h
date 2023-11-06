#pragma once

#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080

#include <string>

namespace Win32 {

    enum WindowType : DWORD {
        STATIC = WS_OVERLAPPED,
        RESIZABLE = WS_OVERLAPPEDWINDOW,
        POPUP = WS_POPUP
    };

    namespace Utils {

        BOOL AddBitmap(CONST WCHAR* szFileName, HDC hWinDC, INT x = 0, INT y = 0);

    }
}