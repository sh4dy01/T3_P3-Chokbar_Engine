#include "Chokbar.h"

#include "IApplication.h"

extern Win32::IApplication* EntryApplication();

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	try
	{
		auto EntryApp = EntryApplication();

		PerGameSettings GameSettings;
		EntryApp->SetupPerGameSettings();

		Logger logger;

		EntryApp->PreInitialize();
		EntryApp->Initialize();

		MSG msg = { 0 };
		while (msg.message != WM_QUIT)
		{
			// If there are Window messages then process them.
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				EntryApp->Update();
			}
		}
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
	

	return 0;
}