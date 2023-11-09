#include "AsteroidLife.h"
#include "AsteroidSpawner.h"

void AsteroidLife::Awake()
{
	LifeComponent::Awake();
}

void AsteroidLife::Start()
{
	m_ParticleRenderer = GameObject::Find("Particles")->GetComponent<ParticleRenderer>();
}

void AsteroidLife::OnDeath()
{
	AsteroidSpawner::OnDestroyedAsteroid(gameObject);
	gameObject->Destroy();
}
