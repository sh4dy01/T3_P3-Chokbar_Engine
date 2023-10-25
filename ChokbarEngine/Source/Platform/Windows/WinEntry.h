#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include "IApplication.h"
#include "Core/DebugUtils.h"

extern Win32::IApplication* EntryApplication();

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	try
	{
		PerGameSettings GameSettings;

		auto EntryApp = EntryApplication();

		EntryApp->SetupPerGameSettings();

		//Logger logger;

		EntryApp->PreInitialize();
		EntryApp->Initialize();

		EntryApp->Run();

		EntryApp->Shutdown();

	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}

	_CrtDumpMemoryLeaks();

	return 0;
}