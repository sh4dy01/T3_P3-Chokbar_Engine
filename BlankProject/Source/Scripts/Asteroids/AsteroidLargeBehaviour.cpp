#include "AsteroidLargeBehaviour.h"
#include <GameObjects/AsteroidSmall.h>


void AsteroidLargeBehaviour::Awake() 
{
	m_Speed = 100.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lifetime = 20.0f;

    m_Rigidbody = gameObject->GetComponent<Rigidbody>();
    gameObject->GetComponent<SphereCollider>()->SetRadius(transform->GetHighestScale());
}

void AsteroidLargeBehaviour::Start() 
{
}

void AsteroidLargeBehaviour::Initialize(Transform* player, const XMFLOAT3& position)
{
	m_Target = player;
    m_Position = position;
    m_Rigidbody->Move(position);

    auto go = GameObject::Instantiate<AsteroidSmall>();

    auto childPos = position;
    childPos.x += 10.f;
    go->GetComponent<Rigidbody>()->Move(childPos);
    go->transform->SetParent(transform);

}

void AsteroidLargeBehaviour::Update()
{
    AsteroidBehaviour::Update();

    XMFLOAT3 targetPos = m_Target->GetPosition();
    XMFLOAT3 currentPos = transform->GetPosition();

    XMFLOAT3 direction;
    XMStoreFloat3(&direction, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&targetPos), XMLoadFloat3(&currentPos))));

    transform->RotateRoll(90.f * TimeManager::GetDeltaTime());

    m_Rigidbody->SetVelocity(XMVectorScale(XMLoadFloat3(&direction), m_Speed));
}
