#include "AsteroidLargeBehaviour.h"

void AsteroidLargeBehaviour::Awake()
{
    m_Speed = 1.0f;
    m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Lifetime = 5.0f;
    m_PlayerLastPositionSet = false;
    m_TimeSinceLastUpdate = 0.0f;
    m_LastPlayerPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);

}

void AsteroidLargeBehaviour::Start()
{
}

void AsteroidLargeBehaviour::Initialize(const XMFLOAT3 direction, float speed, const XMFLOAT3& position)
{
    m_Direction = direction;
    m_Speed = speed;
    m_Position = position;
    transform->SetPosition(position);
}

void AsteroidLargeBehaviour::Update()
{
    XMFLOAT3 asteroidPos = transform->GetPosition();

    if (!m_PlayerLastPositionSet)
    {
        m_LastPlayerPosition = m_PlayerTransform->GetPosition();
        m_PlayerLastPositionSet = true;
    }

    XMFLOAT3 playerPos = m_LastPlayerPosition;

    XMVECTOR asteroidVecPos = XMLoadFloat3(&asteroidPos);
    XMVECTOR playerVecPos = XMLoadFloat3(&playerPos);

    XMVECTOR directionToPlayer = XMVectorSubtract(playerVecPos, asteroidVecPos);
    directionToPlayer = XMVector3Normalize(directionToPlayer);

    XMVECTOR newPosition = XMVectorAdd(asteroidVecPos, XMVectorScale(directionToPlayer, m_Speed * TimeManager::GetDeltaTime()));

    XMFLOAT3 finalPosition;
    XMStoreFloat3(&finalPosition, newPosition);
    transform->SetPosition(finalPosition);

    DestroyAfterATime();
}

void AsteroidLargeBehaviour::DestroyAfterATime()
{
    this->m_Lifetime -= TimeManager::GetDeltaTime();
    if (m_Lifetime <= 0)
    {
        gameObject->Destroy();
    }
}
