#include "AsteroidSmall.h"
#include "Scripts/Asteroids/AsteroidSmallBehaviour.h"

AsteroidSmall::AsteroidSmall() {

    m_Name = "AsteroidSmall";

    m_Lifetime = 5.0f;

    SetType(AsteroidType::SMALL);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    m_Life->InitMaxLife(25);

    AddComponent<AsteroidSmallBehaviour>();

    transform->SetScale(10);
}

AsteroidSmall::~AsteroidSmall()
{
}

void AsteroidSmall::DestroyAfterATime(float m_Lifetime)
{
	m_Lifetime -= TimeManager::GetDeltaTime();
	if (m_Lifetime <= 0)
    {
		GameObject::Destroy();
	}
}