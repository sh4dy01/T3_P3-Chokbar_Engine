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
		//auto player = Chokbar::Engine::GetCoordinator()->GetComponent<PlayerComponent>(entity);
		auto transform = Chokbar::Engine::GetCoordinator()->GetComponent<Transform>(entity);
		//camera

		if (InputHandler::IsKeyHeld('z'))
		{
			transform->RotatePitch(10 * dt);
		}
		if (InputHandler::IsKeyHeld('s'))
		{
			transform->RotatePitch(-10 * dt);
		}
		if (InputHandler::IsKeyHeld('q'))
		{
			transform->RotateYaw(-10 * dt);
		}
		if (InputHandler::IsKeyHeld('d'))
		{
			transform->RotateYaw(10 * dt);
		}
	}
}
