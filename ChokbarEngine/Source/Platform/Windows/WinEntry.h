#pragma once
#include "Engine/Core/CoreDefinitions.h"
#include "IApplication.h"
#include "Engine/Core/DebugUtils.h"

#include "Engine/Engine.h"

extern Win32::IApplication* EntryApplication();

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{

#if defined(DEBUG) | defined(_DEBUG)
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		PerGameSettings GameSettings;

		const auto engine = Engine::GetInstance();
		auto game = EntryApplication();

		game->SetupPerGameSettings();

		engine->Initialize(game);

		engine->Run();

		engine->Shutdown();

		DELPTR(game);

	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif // DEBUG

	return 0;
}