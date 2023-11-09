#include "AsteroidCollisionEvent.h"

#include "GameObjects/Particles/ProjectileParticles.h"

AsteroidCollisionEvent::~AsteroidCollisionEvent()
{
	m_AsteroidLife = nullptr;
	m_ParticleRenderer = nullptr;
}

void AsteroidCollisionEvent::OnAddedComponent()
{
	TriggerCollisionEvent::OnAddedComponent();

	m_AsteroidLife = gameObject->GetComponent<AsteroidLife>();
	GetParticleRenderer();
}

void AsteroidCollisionEvent::OnTriggerEnter(Collider* other)
{
	if (other->gameObject->m_CategoryBitmask.GetLayer() == PROJECTILE)
	{
		if (!m_ParticleRenderer->transform) return;

		m_AsteroidLife->RemoveLife(10);
		
		m_ParticleRenderer->transform->SetPosition(other->gameObject->transform->GetWorldPosition());
		m_ParticleRenderer->AddParticles(20);

		other->gameObject->Destroy();

		DEBUG_LOG("Asteroid life: " << m_AsteroidLife->GetCurrentLife())
	}
}

void AsteroidCollisionEvent::GetParticleRenderer()
{
	m_ParticleRenderer = GameObject::Find("Particles")->GetComponent<ParticleRenderer>();
}
