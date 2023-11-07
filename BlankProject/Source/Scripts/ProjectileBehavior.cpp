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
}

void ProjectileBehavior::Initialize(const XMFLOAT3 direction, float speed, float lifeTime)
{
	m_Direction = direction;
	m_Speed = speed;
	m_LifeTime = lifeTime;
}

void ProjectileBehavior::Update()
{
	transform->Translate(XMVectorScale(XMLoadFloat3(&m_Direction), m_Speed * TimeManager::GetDeltaTime()));

	if (m_LifeTime > 0)
	{
		m_LifeTime -= TimeManager::GetDeltaTime();
	}
	else
	{
		gameObject->Destroy();
	}
}
