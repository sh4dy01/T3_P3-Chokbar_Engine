#include "AsteroidMediumBehaviour.h"


void AsteroidMediumBehaviour::Awake()
{
	m_Speed = 5.0f;
    m_PlayerTransform = nullptr;
}

void AsteroidMediumBehaviour::Start()
{
}

void AsteroidMediumBehaviour::Initialize(Transform* target, float speed)
{
	m_PlayerTransform = target;
	m_Speed = speed;

}

void AsteroidMediumBehaviour::Update()
{
    XMFLOAT3 asteroidPos = transform->GetPosition();
    XMFLOAT3 playerPos = m_PlayerTransform->GetPosition();

    XMVECTOR asteroidVecPos = XMLoadFloat3(&asteroidPos);
    XMVECTOR playerVecPos = XMLoadFloat3(&playerPos);

    XMVECTOR direction = XMVectorSubtract(playerVecPos, asteroidVecPos);

    direction = XMVector3Normalize(direction);

    XMVECTOR velocity = XMVectorScale(direction, m_Speed * TimeManager::GetDeltaTime());

    XMVECTOR newPosition = XMVectorAdd(asteroidVecPos, velocity);

    XMStoreFloat3(&asteroidPos, newPosition);
    transform->SetPosition(asteroidPos);
}