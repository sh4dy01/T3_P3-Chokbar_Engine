#include "AsteroidSmallBehaviour.h"


void AsteroidSmallBehaviour::Awake()
{
	m_Speed = 5.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Lifetime = 5.0f; 
}

void AsteroidSmallBehaviour::Start()
{
}

void AsteroidSmallBehaviour::Update()
{
    transform->Translate(XMVectorScale(XMLoadFloat3(&m_Direction), m_Speed * TimeManager::GetDeltaTime()));

    DestroyAfterATime();
}

void AsteroidSmallBehaviour::Initialize(const XMFLOAT3 direction, float speed, const XMFLOAT3& position)
{
	m_Direction = direction;
	m_Speed = speed;
    m_Position = position;
    transform->SetPosition(position);
}

void AsteroidSmallBehaviour::DestroyAfterATime()
{
    this->m_Lifetime -= TimeManager::GetDeltaTime();
    if (m_Lifetime <= 0)
    {
        gameObject->Destroy();
    }
}