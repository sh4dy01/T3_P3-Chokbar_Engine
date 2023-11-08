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
    m_WaveCooldown = m_WaveCooldownDuration;
}

void AsteroidSpawner::Update()
{
    m_WaveCooldown -= TimeManager::GetDeltaTime();

    if (m_WaveCooldown <= 0.0f)
    {
        m_WaveCount++;
        if (m_WaveCount >= 5)
        {
            m_WaveCount = 0;
        }
        SpawnAsteroidWave(m_WaveCount, m_AsteroidCount);

        m_WaveCooldown = m_WaveCooldownDuration;
    }
}


void AsteroidSpawner::SpawnAsteroidWave(int waveCount, int asteroidCount)
{
    for (int i = 0; i < asteroidCount; i++)
    {
        switch (waveCount)
        {
        case 1:
            SpawnAsteroid(Asteroid::SMALL);
            break;
        case 2:
            SpawnAsteroid(Asteroid::MEDIUM);
            break;
        case 3:
            SpawnAsteroid(Asteroid::LARGE);
            break;
        case 4:
            if (i % 2 == 0)
            {
                SpawnAsteroid(Asteroid::SMALL);
            }
            else
            {
                SpawnAsteroid(Asteroid::MEDIUM);
            }
            break;
        case 5:
            if (i % 4 == 0)
            {
                SpawnAsteroid(Asteroid::SMALL);
            }
            else
            {
                SpawnAsteroid(Asteroid::MEDIUM);
            }
            break;
        default:
            break;
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

void AsteroidSpawner::TestAsteroidLifetime()
{
    for (auto it = m_AliveAsteroids.begin(); it != m_AliveAsteroids.end();)
    {
        GameObject* asteroid = *it;

        bool shouldDestroy = false;

        if (asteroid->HasComponent<AsteroidSmallBehaviour>())
        {
            AsteroidSmallBehaviour* asteroidBehaviour = asteroid->GetComponent<AsteroidSmallBehaviour>();
            shouldDestroy = (asteroidBehaviour->GetLifetime() <= 0.0f);
        }
        else if (asteroid->HasComponent<AsteroidMediumBehaviour>())
        {
            AsteroidMediumBehaviour* asteroidBehaviour = asteroid->GetComponent<AsteroidMediumBehaviour>();
            shouldDestroy = (asteroidBehaviour->GetLifetime() <= 0.0f);
        }
        else if (asteroid->HasComponent<AsteroidLargeBehaviour>())
        {
            AsteroidLargeBehaviour* asteroidBehaviour = asteroid->GetComponent<AsteroidLargeBehaviour>();
            shouldDestroy = (asteroidBehaviour->GetLifetime() <= 0.0f);
        }

        if (shouldDestroy)
        {
            asteroid->Destroy();
            it = m_AliveAsteroids.erase(it);
            m_AliveAsteroidCount--; 
        }
        else
        {
            ++it;
        }
    }
}
