#include "AsteroidMediumBehaviour.h"


void AsteroidMediumBehaviour::Awake()
{
	m_Speed = 2.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void AsteroidMediumBehaviour::Start()
{
	XMFLOAT3 playerPosition = m_PlayerTransform->GetPosition();

	XMFLOAT3 asteroidPosition = transform->GetPosition();
	XMVECTOR dirToPlayer = XMVector3Normalize(
		XMVectorSubtract(XMLoadFloat3(&playerPosition), XMLoadFloat3(&asteroidPosition))
	);

	XMStoreFloat3(&m_Direction, dirToPlayer);
}

void AsteroidMediumBehaviour::Update()
{
	transform->Translate(XMVectorScale(XMLoadFloat3(&m_Direction), m_Speed * TimeManager::GetDeltaTime()));
}
