#include "BlankProject.h"
#include "Asteroid.h"
#include "Engine/Resource.h"

#include "Engine/Resource.h"
#include "Scripts/TriggerCollisionEvent/AsteroidCollisionEvent.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	auto rb = AddComponent<Rigidbody>();
	rb->SetMass(100.f);
	rb->SetBodyType(BodyType::Dynamic);

	auto mesh = AddComponent<MeshRenderer>();
	mesh->Init(SPHERE, TEXTURE);
	mesh->RegisterTexture(Resource::Load<Texture>("Resources/Textures/asteroid.dds"));

	//auto particle = AddComponent<ParticleRenderer>();
	//particle->Init(CUBE, PARTICLE);

	transform->SetScale(20.f);

	auto sc = AddComponent<SphereCollider>();
	sc->SetRadius(20.f);
	//sc->SetCenter({ -4.f, -4.f, -4.f });

	m_Life = AddComponent<AsteroidLife>();
	AddComponent<AsteroidCollisionEvent>();

	m_CategoryBitmask.SetLayer(LayerID::ASTEROID);
	m_CollisionBitmask.SetLayer(LayerID::PLAYER);
	m_CollisionBitmask.AddLayer(LayerID::ASTEROID);
	m_CollisionBitmask.AddLayer(LayerID::PROJECTILE);
	m_CollisionBitmask.AddLayer(LayerID::OBSTACLE);


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


void Asteroid::DestroyAfterATime(float m_Lifetime)
{
	m_Lifetime -= TimeManager::GetDeltaTime();
	if (m_Lifetime <= 0)
	{
		GameObject::Destroy();
	}
}