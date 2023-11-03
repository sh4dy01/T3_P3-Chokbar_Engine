#include "Chokbar.h"
#include "PlayerSystem.h"

#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Components/TransformComponent.h"


void PlayerSystem::Awake()
{
}

void PlayerSystem::Start()
{
}

void PlayerSystem::Update(float dt)
{
	for (const auto entity : m_RegisteredEntities)
	{
		auto player = Engine::GetCoordinator()->GetComponent<PlayerComponent>(entity);
		auto transform = Engine::GetCoordinator()->GetComponent<Transform>(entity);

		
	}
}
