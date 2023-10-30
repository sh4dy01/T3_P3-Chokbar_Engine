#pragma once

#include "Core/CoreDefinitions.h"
#include "Core/PerGameSettings.h"
#include "Common/Logger.h"
#include "Common/Time.h"

#include "Engine/GameTimer.h"
#include "Engine/Managers/InputHandler.h"

#include "Engine/ECS/Base/GameObject.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Core/D3D/Internal/MeshRenderer.h"


#ifdef WIN32
#include "Platform/Windows/Win32Utils.h"
#include "Platform/Windows/Window.h"
#include "Platform/Windows/IApplication.h"
#endif