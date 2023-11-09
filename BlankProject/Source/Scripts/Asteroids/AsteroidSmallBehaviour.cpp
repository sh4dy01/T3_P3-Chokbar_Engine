#include "AsteroidSmallBehaviour.h"


void AsteroidSmallBehaviour::Awake()
{
	m_Speed = 50.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Lifetime = 5.f; 
    m_Rigidbody = gameObject->GetComponent<Rigidbody>();
    gameObject->GetComponent<SphereCollider>()->SetRadius(transform->GetHighestScale());
}

void AsteroidSmallBehaviour::Start()
{
}

void AsteroidSmallBehaviour::Initialize(const XMFLOAT3 direction, const XMFLOAT3& position)
{
	m_Direction = direction;
    m_Position = position;
    m_Rigidbody->Move(position);

    m_Rigidbody->SetVelocity(XMVectorScale(XMLoadFloat3(&m_Direction), m_Speed));
}

void AsteroidSmallBehaviour::Update()
{
    AsteroidBehaviour::Update();
}