#include "Player.h"

#include "Scripts/Player/PlayerMovement.h"
#include "Scripts/Player/PlayerShoot.h"
#include "GameObjects/Wing.h"

Player::Player()
{
	m_Name = "Player";

	AddComponent<Rigidbody>();
	AddComponent<SphereCollider>();

	auto leftWing = GameObject::Instantiate<Wing>();
	leftWing->SetName("LeftWing");
	leftWing->transform->SetPosition(-2, 0, 0);
	leftWing->transform->SetParent(transform);

	auto rightWing = GameObject::Instantiate<Wing>();
	rightWing->SetName("RightWing");
	rightWing->transform->SetPosition(2, 0, 0);
	rightWing->transform->SetParent(transform);

	m_CategoryBitmask.SetLayer(LayerID::PLAYER);
	m_CollisionBitmask.SetLayer(LayerID::ASTEROID);

	AddComponent<PlayerMovement>();
	AddComponent<PlayerShoot>();
}
