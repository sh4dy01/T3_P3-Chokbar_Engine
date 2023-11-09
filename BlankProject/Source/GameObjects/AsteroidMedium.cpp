#include "AsteroidMedium.h"
#include "Scripts/Asteroids/AsteroidMediumBehaviour.h"

AsteroidMedium::AsteroidMedium() {

    m_Name = "AsteroidMedium";

    m_Lifetime = 5.0f;

    AddComponent<AsteroidMediumBehaviour>();

    SetHealth(20);
    SetType(AsteroidType::MEDIUM);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    transform->SetScale(15);
}

AsteroidMedium::~AsteroidMedium()
{
}

void AsteroidMedium::TakeDamage(int damage)
{
    Asteroid::TakeDamage(damage);
}

void AsteroidMedium::DestroyAfterATime(float m_Lifetime)
{
    m_Lifetime -= TimeManager::GetDeltaTime();
    if (m_Lifetime <= 0)
    {
		GameObject::Destroy();
	}
}