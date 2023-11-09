#include "AsteroidMediumBehaviour.h"


void AsteroidMediumBehaviour::Awake()
{

    srand(time(NULL));

    m_Speed = 5.0f;
    m_PlayerTransform = nullptr;
    m_Lifetime = 5.0f;

    m_TeleportationAngle = 66.6f;
    m_TeleportationDistance = 99.f;
    m_TeleportInterval = 2.f;  

    m_TimeSinceLastTeleport = 0.0f;
}

void AsteroidMediumBehaviour::Start()
{
}

void AsteroidMediumBehaviour::Initialize(Transform* target, float speed, const XMFLOAT3& position)
{
	m_PlayerTransform = target;
	m_Speed = speed;
	m_Position = position;
    transform->SetPosition(position);
}

void AsteroidMediumBehaviour::Update()
{
    m_TimeSinceLastTeleport += TimeManager::GetDeltaTime();

    XMFLOAT3 asteroidPos = transform->GetPosition();
    XMFLOAT3 playerPos = m_PlayerTransform->GetPosition();

    XMVECTOR asteroidVecPos = XMLoadFloat3(&asteroidPos);
    XMVECTOR playerVecPos = XMLoadFloat3(&playerPos);

    XMVECTOR directionToPlayer = XMVectorSubtract(playerVecPos, asteroidVecPos);
    directionToPlayer = XMVector3Normalize(directionToPlayer);

    if (m_TimeSinceLastTeleport >= m_TeleportInterval) {
        m_TimeSinceLastTeleport = 0.0f;

        XMVECTOR randomDir = XMVectorSet(
            ((float)rand() / (float)RAND_MAX) * 20.0f,
            ((float)rand() / (float)RAND_MAX) * 20.0f,
            ((float)rand() / (float)RAND_MAX) * 20.0f,
            0.0f
        );
        randomDir = XMVector3Normalize(randomDir);

        if (XMVectorGetX(XMVector3Dot(randomDir, directionToPlayer)) < 0.0f) {
            randomDir = XMVectorNegate(randomDir);
        }

        XMVECTOR teleportOffset = XMVectorScale(randomDir, m_TeleportationDistance);
        XMVECTOR teleportPosition = XMVectorAdd(asteroidVecPos, teleportOffset);

        XMFLOAT3 teleportPos;
        XMStoreFloat3(&teleportPos, teleportPosition);
        transform->SetPosition(teleportPos);

        asteroidVecPos = teleportPosition;
    }

    XMVECTOR newPosition = XMVectorAdd(asteroidVecPos, XMVectorScale(directionToPlayer, m_Speed * TimeManager::GetDeltaTime()));

    XMFLOAT3 finalPosition;
    XMStoreFloat3(&finalPosition, newPosition);
    transform->SetPosition(finalPosition);

    DestroyAfterATime();
}

void AsteroidMediumBehaviour::DestroyAfterATime()
{
    this->m_Lifetime -= TimeManager::GetDeltaTime();
    if (m_Lifetime <= 0)
    {
        gameObject->Destroy();
    }
}