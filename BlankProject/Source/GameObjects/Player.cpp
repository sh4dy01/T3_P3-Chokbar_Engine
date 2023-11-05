#include "BlankProject.h"
#include "Player.h"

#include "Scripts/PlayerMovement.h"
#include "Scripts/PlayerShoot.h"


Player::Player()
{
	m_Name = "Player";

	AddComponent<CameraComponent>();
	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();

	AddComponent<PlayerMovement>();
	AddComponent<PlayerShoot>();
}
