#pragma once

#include "CoreDefinitions.h"
#include "DebugUtils.h"
#include "PerGameSettings.h"

#include "Common/Logger.h"
#include "Common/Time.h"

#include "D3D/Base/MeshType.h"
#include "D3D/Base/MaterialType.h"


// Resources includes
#include "Engine/Resource.h"

// Components includes
#include "Engine/ECS/Base/GameObject.h"
#include "Engine/ECS/Components/Component.h"
#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/ECS/Components/CameraComponent.h"
#include "Engine/ECS/Components/CustomComponent.h"
#include "Engine/ECS/Components/Collision/Rigidbody.h"
#include "Engine/ECS/Components/Collision/SphereCollider.h"

// Managers includes
#include "Engine/Managers/TimeManager.h"
#include "Engine/Managers/CameraManager.h"

// D3D includes
#include "D3D/Renderers/MeshRenderer.h"
#include "D3D/Renderers/ParticleRenderer.h"
#include "D3D/Renderers/SkyRenderer.h"
#include "D3D/Shaders/Texture.h"
#include "D3D/Shaders/Cubemap.h"

#include "Engine/Managers/InputHandler.h"

#ifdef WIN32
#include "Platform/Windows/Win32Utils.h"
#include "Platform/Windows/Window.h"
#include "Platform/Windows/IApplication.h"
#endif