#include "AsteroidSpawner.h"

#include "GameObjects/AsteroidLarge.h"
#include "AsteroidLargeBehaviour.h"
#include "GameObjects/AsteroidMedium.h"
#include "AsteroidMediumBehaviour.h"
#include "GameObjects/AsteroidSmall.h"
#include "AsteroidSmallBehaviour.h"

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
			SpawnAsteroid(Asteroid::SMALL);
			SpawnAsteroid(Asteroid::MEDIUM);
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
	if (type == Asteroid::SMALL) 
	{
		AsteroidSmallBehaviour* asteroid2 = GameObject::Instantiate<AsteroidSmall>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidSmallBehaviour>();
		asteroid2->Initialize(XMFLOAT3(0.0f, 0.0f, -10.0f), 5.0f);
		m_AliveAsteroids.push_back(asteroid2->gameObject);
		m_AliveAsteroidCount++;
	}
	else if (type == Asteroid::MEDIUM) 
	{
		AsteroidMediumBehaviour* asteroid1 = GameObject::Instantiate<AsteroidMedium>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidMediumBehaviour>();
		asteroid1->Initialize(m_PlayerTransform, 20.0f);
		m_AliveAsteroids.push_back(asteroid1->gameObject);
		m_AliveAsteroidCount++;
	}
	else if (type == Asteroid::LARGE) 
	{
		AsteroidLargeBehaviour* asteroid = GameObject::Instantiate<AsteroidLarge>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidLargeBehaviour>();
			asteroid->Initialize(XMFLOAT3(5.0f, 10.0f, -2.0f), 5.0f);
			m_AliveAsteroids.push_back(asteroid->gameObject);
			m_AliveAsteroidCount++;
	}

}
