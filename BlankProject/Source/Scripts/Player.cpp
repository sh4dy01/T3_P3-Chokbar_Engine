#include "BlankProject.h"
#include "Player.h"

Player::Update()
{
	if (InputHandler::IsKeyHeld('z'))
	{
		transform->Translate(0, 0, player->Speed * dt);
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
	if (InputHandler::IsKeyDown(VK_RBUTTON))
	{
		Engine::GetMainCamera()->SetFOV(10.f);
	}
	else if (InputHandler::IsKeyUp(VK_RBUTTON))
	{
		Engine::GetMainCamera()->SetFOV(75.f);
	}
	if (InputHandler::IsKeyHeld(VK_SHIFT))
	{
		transform->Translate(0, -player->Speed * dt, 0, Transform::Space::World);
	}
	if (InputHandler::IsKeyHeld(VK_SPACE))
	{
		transform->Translate(0, player->Speed * dt, 0, Transform::Space::World);
	}

	transform->RotateYaw(InputHandler::GetAxisX() * 50.f * dt, Transform::Space::World);

	transform->RotatePitch(InputHandler::GetAxisY() * 50.f * dt);
}