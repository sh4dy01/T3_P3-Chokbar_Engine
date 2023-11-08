#include "AsteroidMedium.h"
#include "Scripts/Asteroids/AsteroidMediumBehaviour.h"

AsteroidMedium::AsteroidMedium() {

    m_Name = "AsteroidMedium";

    AddComponent<AsteroidMediumBehaviour>();

    SetHealth(20);
    SetType(AsteroidType::MEDIUM);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    transform->SetScale(15);
    transform->SetPosition(0.0f, 50.0f, 50.0f);


}

AsteroidMedium::~AsteroidMedium()
{

}

void AsteroidMedium::TakeDamage(int damage)
{
    Asteroid::TakeDamage(damage);

}