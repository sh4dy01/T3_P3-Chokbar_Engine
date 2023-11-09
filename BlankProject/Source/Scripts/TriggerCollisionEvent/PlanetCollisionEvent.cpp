#include "PlanetCollisionEvent.h"


void PlanetCollisionEvent::OnAddedComponent()
{
	TriggerCollisionEvent::OnAddedComponent();

	m_PlanetLife = gameObject->GetComponent<PlanetLife>();

	GetParticleRenderer();
}

void PlanetCollisionEvent::OnTriggerEnter(Collider* other)
{
	if (other->gameObject->m_CategoryBitmask.GetLayer() == PROJECTILE)
	{
		if (!m_ParticleRenderer->transform) return;
		
		m_ParticleRenderer->transform->SetPosition(other->gameObject->transform->GetWorldPosition());
		m_ParticleRenderer->AddParticles(10);
		m_PlanetLife->RemoveLife(1);

		other->gameObject->Destroy();
	}
}

void PlanetCollisionEvent::GetParticleRenderer()
{
	m_ParticleRenderer = GameObject::Find("Particles")->GetComponent<ParticleRenderer>();
}

PlanetCollisionEvent::PlanetCollisionEvent()
{
}

PlanetCollisionEvent::~PlanetCollisionEvent()
{
	m_ParticleRenderer = nullptr;
}