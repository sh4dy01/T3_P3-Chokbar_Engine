#include "Player.h"

#include "Engine/ECS/Components/CameraComponent.h"
#include "Scripts/TriggerCollisionEvent/PlayerCollisionEvent.h"
#include "Scripts/Player/PlayerLife.h"
#include "Scripts/Player/PlayerMovement.h"
#include "Scripts/Player/PlayerShoot.h"
#include "GameObjects/Wing.h"

Player::Player()
{
	m_Name = "Player";

	AddComponent<CameraComponent>();

	auto rb = AddComponent<Rigidbody>();
	rb->SetMass(1.f);
	rb->SetRestitution(0.8f);

	auto sc = AddComponent<SphereCollider>();
	sc->SetRadius(4.f);
	//sc->SetCenter({ -2.f, -2.f, -2.f });
	
	//transform->SetScale(2.f);

	AddComponent<MeshRenderer>()->Init(SPHERE, SIMPLE);

	auto leftWing = GameObject::Instantiate<Wing>();
	leftWing->SetName("LeftWing");
	leftWing->transform->SetPosition(-2, 0, 2);
	leftWing->transform->SetParent(transform);

	auto rightWing = GameObject::Instantiate<Wing>();
	rightWing->SetName("RightWing");
	rightWing->transform->SetPosition(2, 0, 2);
	rightWing->transform->SetParent(transform);

	m_CategoryBitmask.SetLayer(LayerID::PLAYER);
	m_CollisionBitmask.SetLayer(LayerID::ASTEROID);

	AddComponent<PlayerMovement>();
	AddComponent<PlayerShoot>();
	AddComponent<PlayerLife>()->InitMaxLife(3);
	AddComponent<PlayerCollisionEvent>();
}

Player::~Player()
{
}
