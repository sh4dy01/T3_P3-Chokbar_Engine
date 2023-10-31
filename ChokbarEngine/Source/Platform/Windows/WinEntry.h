#pragma once

#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#ifdef  _DEBUG
	#include <crtdbg.h>
#endif

#include "IApplication.h"
#include "Core/DebugUtils.h"

extern Win32::IApplication* EntryApplication();

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{

#ifdef _DEBUG
	_CrtMemState memStateInit;
	_CrtMemCheckpoint(&memStateInit);
#endif // DEBUG

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

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
	_CrtMemState memStateEnd, memStateDiff;
	_CrtMemCheckpoint(&memStateEnd);

	if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
	{
		MessageBoxA(NULL, "MEMORY LEAKS", "Disclaimer", 0);
	}
#endif // DEBUG

	return 0;
}