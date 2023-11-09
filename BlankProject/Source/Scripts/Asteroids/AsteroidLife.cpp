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

	m_ParticleRenderer->transform->SetPosition(transform->GetWorldPosition());
	m_ParticleRenderer->AddParticles(50);

	//Instantiate particles
}
