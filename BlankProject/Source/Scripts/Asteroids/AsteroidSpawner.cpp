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

    m_LastPlayerPosition = m_PlayerTransform->GetPosition();
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
    XMFLOAT3 playerPosition = m_PlayerTransform->GetPosition();

    float spawnDistance = 500.0f;

    XMFLOAT3 playerForward = m_PlayerTransform->GetForward();

    XMFLOAT3 spawnPosition;
    spawnPosition.x = playerPosition.x + spawnDistance * playerForward.x;
    spawnPosition.y = playerPosition.y;
    spawnPosition.z = playerPosition.z + spawnDistance * playerForward.z;

    if (type == Asteroid::SMALL)
    {
        XMFLOAT3 playerLastPosition = m_LastPlayerPosition;

        XMFLOAT3 direction;
        XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&spawnPosition) - XMLoadFloat3(&playerLastPosition)));

        AsteroidSmallBehaviour* asteroid2 = GameObject::Instantiate<AsteroidSmall>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidSmallBehaviour>();
        asteroid2->Initialize(direction, 30.0f, spawnPosition);
        m_AliveAsteroids.push_back(asteroid2->gameObject);
        m_AliveAsteroidCount++;
    }
    else if (type == Asteroid::MEDIUM)
    {
        AsteroidMediumBehaviour* asteroid1 = GameObject::Instantiate<AsteroidMedium>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidMediumBehaviour>();
        asteroid1->Initialize(m_PlayerTransform, 20.0f, spawnPosition);
        m_AliveAsteroids.push_back(asteroid1->gameObject);
        m_AliveAsteroidCount++;
    }
    else if (type == Asteroid::LARGE)
    {
        XMFLOAT3 playerLastPosition = m_LastPlayerPosition;

        XMFLOAT3 direction;
        XMStoreFloat3(&direction, XMVector3Normalize(XMLoadFloat3(&spawnPosition) - XMLoadFloat3(&playerLastPosition)));

        AsteroidLargeBehaviour* asteroid = GameObject::Instantiate<AsteroidLarge>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidLargeBehaviour>();
        asteroid->Initialize(direction, 30.0f, spawnPosition);
        m_AliveAsteroids.push_back(asteroid->gameObject);
        m_AliveAsteroidCount++;
    }
}

void AsteroidSpawner::TestAsteroidLifetime()
{
    for (auto asteroid : m_AliveAsteroids)
    {
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
            m_AliveAsteroidCount--;
            asteroid->Destroy();
        }
        else
        {
            ++asteroid;
        }
    }
}
