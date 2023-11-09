#include "AsteroidMediumBehaviour.h"


void AsteroidMediumBehaviour::Awake()
{

    srand(time(NULL));

    m_Speed = 1.0f;
    m_Target = nullptr;
    m_Lifetime = 20.0f;

    m_TeleportationAngle = 66.6f;
    m_TeleportationDistance = 99.f;
    m_TeleportInterval = 2.f;  

    m_TimeSinceLastTeleport = 0.0f;

    m_Rigidbody = gameObject->GetComponent<Rigidbody>();
    gameObject->GetComponent<SphereCollider>()->SetRadius(transform->GetHighestScale());
}

void AsteroidMediumBehaviour::Start()
{
}

void AsteroidMediumBehaviour::Initialize(Transform* target, const XMFLOAT3& position)
{
    m_Target = target;
	m_Position = position;
    m_Rigidbody->Move(position);
}

void AsteroidMediumBehaviour::Update()
{
    m_TimeSinceLastTeleport += TimeManager::GetDeltaTime();

    if (m_TimeSinceLastTeleport >= m_TeleportInterval) 
    {
        m_TimeSinceLastTeleport = 0.0f;

    XMFLOAT3 asteroidPos = transform->GetPosition();
    XMFLOAT3 playerPos = m_Target->GetPosition();

    XMVECTOR asteroidVecPos = XMLoadFloat3(&asteroidPos);
    XMVECTOR playerVecPos = XMLoadFloat3(&playerPos);

    XMVECTOR directionToPlayer = XMVectorSubtract(playerVecPos, asteroidVecPos);
    directionToPlayer = XMVector3Normalize(directionToPlayer);

   

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
        m_Rigidbody->Move(teleportPos);

        asteroidVecPos = teleportPosition;

        XMVECTOR newPosition = XMVectorAdd(asteroidVecPos, XMVectorScale(directionToPlayer, m_Speed * TimeManager::GetDeltaTime()));
        XMFLOAT3 finalPosition;
        XMStoreFloat3(&finalPosition, newPosition);
        m_Rigidbody->Move(finalPosition);
    }

  


    AsteroidBehaviour::DestroyAfterATime();
}
