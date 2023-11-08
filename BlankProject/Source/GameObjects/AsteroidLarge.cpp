#include "AsteroidLarge.h"

AsteroidLarge::AsteroidLarge() {
    m_Name = "AsteroidLarge"; 

    AddComponent<Rigidbody>(); 

    auto mesh = AddComponent<MeshRenderer>(); 
    mesh->Init(SPHERE, SIMPLE);

    AddComponent<SphereCollider>(); 

    SetHealth(20);
    SetType(AsteroidType::LARGE);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    SetScale(DirectX::XMFLOAT3(6.0f, 6.0f, 6.0f));

    mesh = nullptr;
}

AsteroidLarge::~AsteroidLarge() 
{
  
}

void AsteroidLarge::TakeDamage(int damage) 
{
    Asteroid::TakeDamage(damage);

}