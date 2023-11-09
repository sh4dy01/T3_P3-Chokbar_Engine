#include "AsteroidLargeBehaviour.h"


void AsteroidLargeBehaviour::Awake() 
{

	m_Speed = 0.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lifetime = 100.0f;

    m_Rigidbody = gameObject->GetComponent<Rigidbody>();
    gameObject->GetComponent<SphereCollider>()->SetRadius(transform->GetHighestScale());
}

void AsteroidLargeBehaviour::Start() 
{
}

void AsteroidLargeBehaviour::Initialize(const XMFLOAT3 direction, float speed, const XMFLOAT3& position)
{
	m_Direction = direction;
	m_Speed = speed;
    m_Position = position;
    m_Rigidbody->Move(position);

    m_Rigidbody->SetVelocity(XMVectorScale(XMLoadFloat3(&m_Direction), m_Speed));
}

void AsteroidLargeBehaviour::Update()
{
    DestroyAfterATime();
}
