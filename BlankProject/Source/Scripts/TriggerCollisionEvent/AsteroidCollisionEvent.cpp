#include "AsteroidCollisionEvent.h"

#include "GameObjects/Particles/ProjectileParticles.h"

AsteroidCollisionEvent::~AsteroidCollisionEvent()
{
	m_AsteroidLife = nullptr;
}

void AsteroidCollisionEvent::OnAddedComponent()
{
	TriggerCollisionEvent::OnAddedComponent();

	m_AsteroidLife = gameObject->GetComponent<AsteroidLife>();
}

void AsteroidCollisionEvent::OnTriggerEnter(Collider* other)
{
	if (other->gameObject->m_CategoryBitmask.GetLayer() == PROJECTILE)
	{
		m_AsteroidLife->RemoveLife(10);
		
		const auto particles = GameObject::Instantiate<ProjectileParticles>()->GetComponent<ParticleRenderer>();
		particles->transform->SetPosition(other->gameObject->transform->GetWorldPosition());
		particles->AddParticles(100);
		particles->Play();

		other->gameObject->Destroy();

		DEBUG_LOG("Asteroid life: " << m_AsteroidLife->GetCurrentLife());
	}
}
