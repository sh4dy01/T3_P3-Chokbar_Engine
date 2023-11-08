#pragma once

#include "GameObjects/Asteroid.h"

class AsteroidSpawner : public CustomComponent
{
public:

	AsteroidSpawner() = default;
	~AsteroidSpawner() override;

	void Awake() override;
	void Update() override;

	void SpawnAsteroidWave(int m_WaveCount, int m_AsteroidCount);
	void SpawnAsteroid(Asteroid::AsteroidType type);
	void TestAsteroidLifetime();

private:

	Transform* m_PlayerTransform;
	XMFLOAT3 m_LastPlayerPosition;

	std::vector<GameObject*> m_AliveAsteroids;

	int m_AsteroidCount = 10;
	int m_AliveAsteroidCount = 0;
	int m_WaveCount = 0;

	float m_WaveCooldown = 0.0f;
	float m_WaveCooldownDuration = 5.0f;

	float m_SpawnTimer = 1.0f;
	float m_Timer = 0.0f;
};

