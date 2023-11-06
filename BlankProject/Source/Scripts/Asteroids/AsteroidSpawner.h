#pragma once

class AsteroidSpawner : public CustomComponent
{
public:

	AsteroidSpawner() = default;
	~AsteroidSpawner() override;

	void Awake() override;
	void Update() override;

	void SpawnAsteroidWave(int targetCount);
	void SpawnAsteroid();


private:

	Transform* m_PlayerTransform;

	std::vector<GameObject*> m_AliveAsteroids;

	int m_AsteroidCount = 10;
	int m_AliveAsteroidCount = 0;
	int m_WaveCount = 0;

	float m_SpawnTimer = 1.0f;
	float m_Timer = 0.0f;
};

