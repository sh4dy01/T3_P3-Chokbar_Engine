#include "AsteroidBehaviour.h"
#include "AsteroidSpawner.h"

void AsteroidBehaviour::Awake()
{
	m_Speed = 0.0f;
	m_Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Lifetime = 0.0f; 
	m_Rigidbody = gameObject->GetComponent<Rigidbody>();
	gameObject->GetComponent<SphereCollider>()->SetRadius(transform->GetHighestScale());
}

void AsteroidBehaviour::Start()
{
}


void AsteroidBehaviour::Update()
{
	DestroyAfterATime();
}

void AsteroidBehaviour::DestroyAfterATime()
{
	m_Lifetime -= TimeManager::GetDeltaTime();
	if (m_Lifetime <= 0.f)
	{
		AsteroidSpawner::OnDestroyedAsteroid(gameObject);
		gameObject->Destroy();
	}
}