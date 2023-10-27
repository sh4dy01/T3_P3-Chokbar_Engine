#include "Chokbar.h"
#include "PlayerSystem.h"

#include "Engine/InputHandler.h"
#include "Engine/ECS/Components/PlayerComponent.h"
#include "Engine/ECS/Components/TransformComponent.h"

namespace Chokbar {

	void PlayerSystem::Update(float deltaTime)
	{
		for (auto& entity : m_RegisteredEntities)
		{
			auto player = Engine::GetCoordinator().GetComponent<PlayerComponent>(entity);
			auto transform = Engine::GetCoordinator().GetComponent<Transform>(entity);
			//camera 

			if (InputHandler::IsKeyHeld('z'))
			{
				transform->Translate(;
			}
			if (InputHandler::IsKeyHeld('s'))
			{
				transform.position.y -= 1.0f * deltaTime;
			}
			if (InputHandler::IsKeyHeld('q'))
			{
				transform.position.x -= 1.0f * deltaTime;
			}
			if (InputHandler::IsKeyHeld('d'))
			{
				transform.position.x += 1.0f * deltaTime;
			}

			transform.Rotate(0.0f, InputHandler::GetAxisX() * 2, 0);
		}
	}

}
