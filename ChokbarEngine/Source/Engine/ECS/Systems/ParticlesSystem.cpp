#include "Chokbar.h"

#include "ParticlesSystem.h"

using namespace DirectX;

ParticleSystem::ParticleSystem()
{
	srand(time(NULL));
	m_ParticlesRenderer = new ParticlesRenderer();
}

ParticleSystem::~ParticleSystem()
{
	for (auto& p : m_Particles)
		DELPTR(p);
}

void ParticleSystem::Awake()
{
	float randomLifeTime = 0.0f;
	float randomSpeed = 0.0f;
	XMFLOAT3 randomDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	for (auto& p : m_Particles)
	{
		randomLifeTime = (float)(rand() % 300) / 100.0f;
		randomSpeed = (float)(rand() % 200) / 100.0f;
		randomDirection = XMFLOAT3((float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f, (float)(rand() % 100) / 100.0f);

		p = new Particle();
		p->Init(randomLifeTime, randomSpeed, randomDirection);
	}

	m_ParticlesRenderer->CreateGlobalMesh(m_Particles);
}

void ParticleSystem::Start()
{
	
}

void ParticleSystem::Update(float deltaTime)
{
	for (auto& p : m_Particles)
	{
		if (!p->IsActive()) return;

		p->Update(deltaTime);

		if (!p->IsAlive() && p->IsActive()) p->ToggleActivity();
	}

	m_ParticlesRenderer->CreateGlobalMesh(m_Particles);
}