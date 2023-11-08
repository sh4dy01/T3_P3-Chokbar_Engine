#include "AsteroidLarge.h"
#include "Scripts/Asteroids/AsteroidLargeBehaviour.h"

AsteroidLarge::AsteroidLarge() {

    m_Name = "AsteroidLarge"; 

    AddComponent<AsteroidLargeBehaviour>();

    SetHealth(20);
    SetType(AsteroidType::LARGE);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    
    transform->SetScale(25);
    transform->SetPosition(0.0f, 0.0f, 50.0f);
    transform->Rotate(0.0f, 0.0f, 0.0f);

}

AsteroidLarge::~AsteroidLarge() 
{
  
}

void AsteroidLarge::TakeDamage(int damage) 
{
    Asteroid::TakeDamage(damage);

}