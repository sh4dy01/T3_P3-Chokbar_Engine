#pragma once

#include "Core/CoreDefinitions.h"
#include "Core/DebugUtils.h"
#include "Core/PerGameSettings.h"

#include "Common/Logger.h"
#include "Common/Time.h"

#include "Core/D3D/MeshType.h"
#include "Core/D3D/MaterialType.h"

#include "Engine/Engine.h"
#include "Engine/Managers/TimeManager.h"
#include "Engine/Managers/CameraManager.h"

#include "Engine/ECS/Base/GameObject.h"
#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Components/CustomComponent.h"
#include "Engine/ECS/Components/Collision/Rigidbody.h"
#include "Engine/ECS/Components/Collision/SphereCollider.h"

#include "Core/D3D/Internal/MeshRenderer.h"

#include "Engine/Managers/InputHandler.h"

#ifdef WIN32
#include "Platform/Windows/Win32Utils.h"
#include "Platform/Windows/Window.h"
#include "Platform/Windows/IApplication.h"
#endif