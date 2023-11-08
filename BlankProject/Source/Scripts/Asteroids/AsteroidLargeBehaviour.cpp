#include "AsteroidLargeBehaviour.h"


void AsteroidLargeBehaviour::Awake() 
{

	m_Speed = 0.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lifetime = 5.0f;
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
    if (m_PlayerTransform)
    {
        XMFLOAT3 currentPlayerPosition = m_PlayerTransform->GetPosition();

        XMFLOAT3 directionToPlayer;
        XMStoreFloat3(&directionToPlayer, XMVector3Normalize(XMLoadFloat3(&currentPlayerPosition) - XMLoadFloat3(&m_Position)));

        transform->Translate(XMVectorScale(XMLoadFloat3(&directionToPlayer), m_Speed * TimeManager::GetDeltaTime()));
    }

    float deltaTime = TimeManager::GetDeltaTime();
    float rotationSpeedX = 100.0f * deltaTime;

    transform->Rotate(rotationSpeedX, 0, 0);

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