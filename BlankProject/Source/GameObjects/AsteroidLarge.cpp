#include "AsteroidLarge.h"
#include "Scripts/Asteroids/AsteroidLargeBehaviour.h"

AsteroidLarge::AsteroidLarge() {

    m_Name = "AsteroidLarge"; 

    m_Lifetime = 5.0f;

    AddComponent<AsteroidLargeBehaviour>();

    m_Life->InitMaxLife(200);
    SetType(AsteroidType::LARGE);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    
    transform->SetScale(25);
    transform->Rotate(0.0f, 0.0f, 0.0f);
}

AsteroidLarge::~AsteroidLarge() 
{
}

void AsteroidLarge::DestroyAfterATime(float m_Lifetime)
{
    m_Lifetime -= TimeManager::GetDeltaTime();
    if (m_Lifetime <= 0)
    {
        GameObject::Destroy();
    }
}