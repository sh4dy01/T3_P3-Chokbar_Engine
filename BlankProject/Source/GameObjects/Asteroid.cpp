#include "BlankProject.h"
#include "Asteroid.h"

Asteroid::Asteroid()
{
	m_Name = "Asteroid";

	AddComponent<Rigidbody>();

	auto mesh = AddComponent<MeshRenderer>();
	mesh->Init(SPHERE, SIMPLE);

	AddComponent<SphereCollider>();

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

void Asteroid::SetDirection(const DirectX::XMFLOAT3& direction) 
{
    m_Direction = direction;
}

DirectX::XMFLOAT3 Asteroid::GetDirection() const 
{
    return m_Direction;
}

void Asteroid::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_Scale = scale;
}

DirectX::XMFLOAT3 Asteroid::GetScale() const
{
	return m_Scale;
}

void Asteroid::Update() 
{
}

void Asteroid::TakeDamage(int damage) 
{
    m_Health -= damage;
    if (m_Health <= 0) {
        delete this;
    }
}