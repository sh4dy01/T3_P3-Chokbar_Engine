#include "AsteroidMediumBehaviour.h"


void AsteroidMediumBehaviour::Awake()
{
    srand(time(NULL));

    m_Speed = 1.0f;
    m_Target = nullptr;
    m_Lifetime = 30.0f;

    m_TeleportationAngle = 66.6f;
    m_TeleportationDistance = 150.f;
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

        float angleVariation = m_TeleportationAngle * (float(rand()) / RAND_MAX - 0.5f) * 2.0f; 
        float rotationAngleRad = XMConvertToRadians(angleVariation);

        XMVECTOR rotationAxis = XMVector3Cross(directionToPlayer, XMVectorSet(0, 1, 0, 0));
        rotationAxis = XMVector3Normalize(rotationAxis);
        XMMATRIX rotationMatrix = XMMatrixRotationAxis(rotationAxis, rotationAngleRad);

        XMVECTOR teleportDirection = XMVector3TransformNormal(directionToPlayer, rotationMatrix);
        teleportDirection = XMVector3Normalize(teleportDirection);

        XMVECTOR displacement = XMVectorScale(teleportDirection, m_TeleportationDistance);

        m_Rigidbody->Move(displacement, Transform::Space::World);
    }

    AsteroidBehaviour::DestroyAfterATime();
}
