#include "AsteroidSmall.h"
#include "Scripts/Asteroids/AsteroidSmallBehaviour.h"

AsteroidSmall::AsteroidSmall() {

    m_Name = "AsteroidSmall";

    AddComponent<AsteroidSmallBehaviour>();

    SetHealth(20);
    SetType(AsteroidType::SMALL);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    transform->SetScale(5);
    transform->SetPosition(50.0f, 0.0f, 50.0f);

}

AsteroidSmall::~AsteroidSmall()
{

}

void AsteroidSmall::TakeDamage(int damage)
{
    Asteroid::TakeDamage(damage);

}