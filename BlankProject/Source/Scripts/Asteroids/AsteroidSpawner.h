#pragma once

#include "GameObjects/Asteroid.h"

class AsteroidSpawner : public CustomComponent
{
public:

	AsteroidSpawner() = default;
	~AsteroidSpawner() override;

	void Awake() override;
	void Update() override;

	void TriggerNextWave();
	void CheckIfWaveSpawned();

	void SpawnAsteroidWave();
	void SpawnAsteroid(Asteroid::AsteroidType type);
	static void OnDestroyedAsteroid(GameObject* go);


private:

	Transform* m_PlayerTransform;
	XMFLOAT3 m_LastPlayerPosition;

	int m_TargetAsteroidCount = 10;
	inline static int m_AliveAsteroidCount = 0;
	int m_SpawnedAsteroidCount = 0;
	int m_WaveCount = 0;

	float m_Timer = 0.0f;
	float m_SpawnTimerDuration = 1.0f;

	bool m_IsSpawning = false;
};