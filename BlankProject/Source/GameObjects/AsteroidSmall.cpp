#include "AsteroidSmall.h"
#include "Scripts/Asteroids/AsteroidSmallBehaviour.h"

AsteroidSmall::AsteroidSmall() {

    m_Name = "AsteroidSmall";

    m_Lifetime = 5.0f;

    AddComponent<AsteroidSmallBehaviour>();

    SetHealth(20);
    SetType(AsteroidType::SMALL);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    transform->SetScale(5);
}

AsteroidSmall::~AsteroidSmall()
{
}

void AsteroidSmall::TakeDamage(int damage)
{
    Asteroid::TakeDamage(damage);
}

void AsteroidSmall::DestroyAfterATime(float m_Lifetime)
{
	this->m_Lifetime -= TimeManager::GetDeltaTime();
	if (m_Lifetime <= 0)
    {
		GameObject::Destroy();
	}
}