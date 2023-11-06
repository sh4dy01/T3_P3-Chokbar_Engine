#include "Player.h"

#include "Engine/ECS/Components/CameraComponent.h"
#include "Scripts/Player/PlayerMovement.h"
#include "Scripts/Player/PlayerShoot.h"


Player::Player()
{
	m_Name = "Player";

	AddComponent<CameraComponent>();
	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();

	AddComponent<PlayerMovement>();
	AddComponent<PlayerShoot>();
}
