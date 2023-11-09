#include "BlankProject.h"
#include "Asteroid.h"
#include "Engine/Resource.h"

#include "../Scripts/Asteroids/AsteroidLife.h"
#include "Engine/Resource.h"
#include "Scripts/TriggerCollisionEvent/AsteroidCollisionEvent.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	auto rb = AddComponent<Rigidbody>();
	rb->SetMass(100.f);
	rb->SetBodyType(BodyType::Static);

	auto mesh = AddComponent<MeshRenderer>();
	mesh->Init(SPHERE, TEXTURE);
	mesh->RegisterTexture(Resource::Load<Texture>("Resources/Textures/mars.dds"));

	transform->SetScale(2.f);

	auto sc = AddComponent<SphereCollider>();
	sc->SetRadius(2.f);
	// sc->SetCenter({ -4.f, -4.f, -4.f });

	AddComponent<AsteroidLife>();
	AddComponent<AsteroidCollisionEvent>();

	m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
	m_CollisionBitmask.SetLayer(LayerID::PLAYER);
	m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
	m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);

	mesh = nullptr;
}

Asteroid::~Asteroid()
{
}

Asteroid::AsteroidType Asteroid::GetType() const
{
	return m_Type;
}

void Asteroid::SetType(AsteroidType type)
{
	m_Type = type;
}

int Asteroid::GetHealth() const
{
	return m_Health;
}

void Asteroid::SetHealth(int health)
{
	m_Health = health;
}

void Asteroid::SetDirection(const DirectX::XMFLOAT3 &direction)
{
	m_Direction = direction;
}

DirectX::XMFLOAT3 Asteroid::GetDirection() const
{
	return m_Direction;
}

void Asteroid::Update()
{
}

void Asteroid::TakeDamage(int damage)
{
	m_Health -= damage;
	if (m_Health <= 0)
	{
		delete this;
	}
}

void Asteroid::DestroyAfterATime(float m_Lifetime)
{
	m_Lifetime -= TimeManager::GetDeltaTime();
	if (m_Lifetime <= 0)
	{
		GameObject::Destroy();
	}
}