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
    m_SpawnTimerDuration = 1.0f;
    m_AliveAsteroidCount = 0;
    m_Timer = m_SpawnTimerDuration;
    m_WaveCount = 5;
    m_TargetAsteroidCount = 10;
    m_IsSpawning = true;
}

void AsteroidSpawner::Update()
{
    if (m_IsSpawning)
        SpawnAsteroidWave();

    if (m_IsSpawning && m_AliveAsteroidCount >= m_TargetAsteroidCount)
        m_IsSpawning = false;

   // CheckIfWaveSpawned();

    if (!m_IsSpawning && m_AliveAsteroidCount <= 0)
        TriggerNextWave();

    m_LastPlayerPosition = m_PlayerTransform->GetPosition();
}

void AsteroidSpawner::TriggerNextWave()
{
    m_WaveCount++;
    m_IsSpawning = true;
    m_AliveAsteroidCount = 0;
    m_Timer = m_SpawnTimerDuration;
}

void AsteroidSpawner::CheckIfWaveSpawned()
{
   
}

void AsteroidSpawner::SpawnAsteroidWave()
{
    if (m_Timer <= 0.0f)
    {
        switch (m_WaveCount)
        {
        case 1:
            m_TargetAsteroidCount = 2;
            SpawnAsteroid(Asteroid::SMALL);
            break;
        case 2:
            m_TargetAsteroidCount = 3;
            SpawnAsteroid(Asteroid::MEDIUM);
            break;
        case 3:
            if (m_AliveAsteroidCount % 4 == 0)
            {
                SpawnAsteroid(Asteroid::SMALL);
            }
            else
            {
                SpawnAsteroid(Asteroid::MEDIUM);
            }
            break;
        case 4:
            m_TargetAsteroidCount = 4;
            if (m_AliveAsteroidCount % 2 == 0)
            {
                SpawnAsteroid(Asteroid::SMALL);
            }
            else
            {
                SpawnAsteroid(Asteroid::MEDIUM);
            }
            break;
        case 5:
            m_TargetAsteroidCount = 1;
            SpawnAsteroid(Asteroid::LARGE);
            break;
        default:
            break;
        }

        m_Timer = m_SpawnTimerDuration;
    }
    else
    {
        m_Timer -= TimeManager::GetDeltaTime();
    }
}

void AsteroidSpawner::SpawnAsteroid(Asteroid::AsteroidType type)
{
    XMFLOAT3 playerPosition = m_PlayerTransform->GetPosition();
    XMFLOAT3 playerForward = m_PlayerTransform->GetForward();

    float spawnDistance = 500.0f;
    float maxAngle = 30.0f;

   
    float randomAngle = (rand() % (2 * static_cast<int>(maxAngle * 100.0f))) / 100.0f - maxAngle;

    XMMATRIX rotationMatrix = XMMatrixRotationY(XMConvertToRadians(randomAngle));
    XMFLOAT3 direction;
    XMStoreFloat3(&direction, XMVector3Transform(XMLoadFloat3(&playerForward), rotationMatrix));

    XMFLOAT3 spawnPosition;
    spawnPosition.x = playerPosition.x + spawnDistance * direction.x;
    spawnPosition.y = playerPosition.y + spawnDistance * direction.y;
    spawnPosition.z = playerPosition.z + spawnDistance * direction.z;
    if (type == Asteroid::SMALL)
    {
        XMFLOAT3 playerLastPosition = m_LastPlayerPosition;

        XMFLOAT3 direction;
        XMStoreFloat3(&direction, XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&playerLastPosition), XMLoadFloat3(&spawnPosition))));

        AsteroidSmallBehaviour* asteroid2 = GameObject::Instantiate<AsteroidSmall>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidSmallBehaviour>();
        asteroid2->Initialize(direction, spawnPosition);
    }
    else if (type == Asteroid::MEDIUM)
    {
        AsteroidMediumBehaviour* asteroid1 = GameObject::Instantiate<AsteroidMedium>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidMediumBehaviour>();
        asteroid1->Initialize(m_PlayerTransform, spawnPosition);
    }
    else if (type == Asteroid::LARGE)
    {
        AsteroidLargeBehaviour* asteroid = GameObject::Instantiate<AsteroidLarge>("Asteroid " + std::to_string(m_AliveAsteroidCount))->GetComponent<AsteroidLargeBehaviour>();
        asteroid->Initialize(m_PlayerTransform, spawnPosition);
    }

    m_AliveAsteroidCount++;   
}

void AsteroidSpawner::OnDestroyedAsteroid(GameObject* go)
{
    m_AliveAsteroidCount--;
}
