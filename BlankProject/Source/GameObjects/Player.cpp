#include "BlankProject.h"
#include "Player.h"

#include "Scripts/PlayerComponent.h"

Player::Player()
{
	m_Name = "Player";
	AddComponent<PlayerComponent>();
	AddComponent<CameraComponent>();
	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();
}
