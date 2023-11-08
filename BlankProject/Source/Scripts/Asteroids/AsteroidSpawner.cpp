#include "AsteroidSpawner.h"

#include "GameObjects/AsteroidLarge.h"
#include "AsteroidLargeBehaviour.h"

AsteroidSpawner::~AsteroidSpawner()
{

}

void AsteroidSpawner::Awake()
{
	m_PlayerTransform = GameObject::Find("Player")->transform;
}

void AsteroidSpawner::Update()
{
	if (m_AliveAsteroidCount == 0)
	{
		SpawnAsteroidWave(m_AsteroidCount);
	}
}

void AsteroidSpawner::SpawnAsteroidWave(int targetCount)
{
	for (int i = 0; i < targetCount; i++)
	{
		if (m_Timer > m_SpawnTimer)
		{
			SpawnAsteroid(Asteroid::LARGE);
			m_Timer = 0.0f;
		}
		else
		{
			m_Timer += TimeManager::GetDeltaTime();
		}
	}
}

void AsteroidSpawner::SpawnAsteroid(Asteroid::AsteroidType type)
{
	switch (type)
	{
		
	case Asteroid::LARGE:
		auto asteroid = GameObject::Instantiate<AsteroidLarge>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidLargeBehaviour>();
		asteroid->Initialize(XMFLOAT3(0.0f, 0.0f, 0.0f), 1.0f);
		m_AliveAsteroids.push_back(asteroid->gameObject);
		m_AliveAsteroidCount++;
		break;
	}

	
}
