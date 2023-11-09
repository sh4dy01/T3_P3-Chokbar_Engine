#include "AsteroidSmall.h"
#include "Scripts/Asteroids/AsteroidSmallBehaviour.h"

AsteroidSmall::AsteroidSmall() {

    m_Name = "AsteroidSmall";

    m_Lifetime = 3.5f;

    SetType(AsteroidType::SMALL);
    SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
    m_Life->InitMaxLife(25);

    AddComponent<AsteroidSmallBehaviour>();

    transform->SetScale(10);
    GetComponent<SphereCollider>()->SetRadius(10);
}

AsteroidSmall::~AsteroidSmall()
{
}
