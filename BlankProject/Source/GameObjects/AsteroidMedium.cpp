#include "AsteroidMedium.h"
#include "Scripts/Asteroids/AsteroidMediumBehaviour.h"

AsteroidMedium::AsteroidMedium() {

    m_Name = "AsteroidMedium";

    m_Lifetime = 5.0f;

    AddComponent<AsteroidMediumBehaviour>();

    m_Life->InitMaxLife(75);
    SetType(AsteroidType::MEDIUM);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    transform->SetScale(15);
    GetComponent<SphereCollider>()->SetRadius(15);
}

AsteroidMedium::~AsteroidMedium()
{
}