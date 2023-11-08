#include "AsteroidSmallBehaviour.h"


void AsteroidSmallBehaviour::Awake()
{
	m_Speed = 5.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void AsteroidSmallBehaviour::Start()
{
}

void AsteroidSmallBehaviour::Initialize(const XMFLOAT3 direction, float speed)
{
	m_Direction = direction;
	m_Speed = speed;
}

void AsteroidSmallBehaviour::Update()
{
	transform->Translate(XMVectorScale(XMLoadFloat3(&m_Direction), m_Speed * TimeManager::GetDeltaTime()));
}

