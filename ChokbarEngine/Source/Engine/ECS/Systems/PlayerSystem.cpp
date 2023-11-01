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
			transform->Translate(0, 0, -1 * dt);
		}
		if (InputHandler::IsKeyHeld('s'))
		{
			transform->Translate(0, 0, 1 * dt);
		}
		if (InputHandler::IsKeyHeld('q'))
		{
			transform->Translate(1 * dt, 0, 0);
		}
		if (InputHandler::IsKeyHeld('d'))
		{
			transform->Translate(-1 * dt, 0, 0);
		}
		if (InputHandler::IsKeyHeld(VK_SHIFT))
		{
			transform->Translate(0, 1 * dt, 0);
		}
		if (InputHandler::IsKeyHeld(VK_SPACE))
		{
			transform->Translate(0, -1 * dt, 0);
		}

	}
}
