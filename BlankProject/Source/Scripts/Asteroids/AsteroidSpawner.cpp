#include "AsteroidSpawner.h"

#include "GameObjects/Asteroid.h"


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
			SpawnAsteroid();
			m_Timer = 0.0f;
		}
		else
		{
			m_Timer += TimeManager::GetDeltaTime();
		}
	}
}

void AsteroidSpawner::SpawnAsteroid()
{
	m_AliveAsteroids.push_back(GameObject::Instantiate<Asteroid>("Asteroid " + std::to_string(m_AliveAsteroidCount)));
	m_AliveAsteroidCount++;
}
