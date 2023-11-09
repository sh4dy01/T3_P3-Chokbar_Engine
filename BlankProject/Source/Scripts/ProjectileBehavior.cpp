#include "BlankProject.h"
#include "ProjectileBehavior.h"


void ProjectileBehavior::Awake()
{
	m_Speed = 0.0f;
	m_LifeTime = 1.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void ProjectileBehavior::Start()
{
	m_pRigidbody = gameObject->GetComponent<Rigidbody>();
}

void ProjectileBehavior::Initialize(const XMFLOAT3 direction, float speed, float lifeTime)
{
	m_Speed = speed;

	m_Direction = direction;
	m_Direction.x *= m_Speed;
	m_Direction.y *= m_Speed;
	m_Direction.z *= m_Speed;

	m_LifeTime = lifeTime;

	m_pRigidbody->SetVelocity(m_Direction);
}

void ProjectileBehavior::Update()
{
	if (m_LifeTime > 0)
	{
		m_LifeTime -= TimeManager::GetDeltaTime();
	}
	else
	{
		gameObject->Destroy();
	}
}