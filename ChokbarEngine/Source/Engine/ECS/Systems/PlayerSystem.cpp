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
		auto player = Chokbar::Engine::GetCoordinator()->GetComponent<PlayerComponent>(entity);
		auto transform = Chokbar::Engine::GetCoordinator()->GetComponent<Transform>(entity);
		//camera

		if (InputHandler::IsKeyHeld('z'))
		{
			transform->Translate(0, 0, player->Speed * dt);
			//transform->
		}
		if (InputHandler::IsKeyHeld('s'))
		{
			transform->Translate(0, 0, -player->Speed * dt);
		}
		if (InputHandler::IsKeyHeld('q'))
		{
			transform->Translate(-player->Speed * dt, 0, 0);
		}
		if (InputHandler::IsKeyHeld('d'))
		{
			transform->Translate(player->Speed * dt, 0, 0);
		}
		if (InputHandler::IsKeyHeld(VK_SHIFT))
		{
			transform->TranslateWorld(0, -player->Speed * dt, 0);
		}
		if (InputHandler::IsKeyHeld(VK_SPACE))
		{
			transform->TranslateWorld(0, player->Speed * dt, 0);
		}

		DEBUG_LOG(std::to_string(transform->GetEulerAngles().x));

		transform->RotatePitch(InputHandler::GetAxisY() * 50.f * dt);

		transform->RotateYaw(InputHandler::GetAxisX() * 50.f * dt);
	}
}
