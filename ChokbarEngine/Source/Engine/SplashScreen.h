#pragma once

#include "../Platform/Windows/Window.h"

namespace SplashScreen {

	void CHOKBAR_API Open();
	void CHOKBAR_API Close();
	void CHOKBAR_API SetMessage(const WCHAR* message);

}

class CHOKBAR_API SplashWindow : public Win32::Window {

public:

	SplashWindow();
	~SplashWindow();

	LRESULT MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;


private:

	WCHAR m_OutputMessage[MAX_NAME_STRING];

};