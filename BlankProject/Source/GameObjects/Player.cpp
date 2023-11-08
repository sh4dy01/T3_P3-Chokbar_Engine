#include "Player.h"

#include "Engine/ECS/Components/CameraComponent.h"
#include "Scripts/Player/PlayerMovement.h"
#include "Scripts/Player/PlayerShoot.h"


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
	AddComponent<PlayerMovement>();
	AddComponent<PlayerShoot>();
}
