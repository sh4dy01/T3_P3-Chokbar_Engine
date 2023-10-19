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

		EntryApp->Run();

	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
	

	return 0;
}