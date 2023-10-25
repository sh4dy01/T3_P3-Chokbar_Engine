#include "Chokbar.h"
#include "TransformSystem.h"
#include "Engine/ECS/Base/Coordinator.h"
#include "Engine/Engine.h"

namespace Chokbar {

	void TransformSystem::Update()
	{
		for (auto entity : m_AllEntities)
		{
			auto& transform = Engine::GetCoordinator().GetComponent<Transform>(entity);
			auto& position = transform.Position;
			auto& rotation = transform.Rotation;
			auto& scale = transform.Scale;
		}
	}
}

