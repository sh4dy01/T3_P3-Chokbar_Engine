#pragma once

#include "Core/CoreDefinitions.h"
#include "Core/PerGameSettings.h"
#include "Common/Logger.h"
#include "Common/Time.h"

#include "Engine/GameTimer.h"

#ifdef WIN32
	#include "Platform/Windows/Win32Utils.h"
	#include "Platform/Windows/SubObject.h"
	#include "Platform/Windows/Window.h"
	#include "Platform/Windows/IApplication.h"
#endif