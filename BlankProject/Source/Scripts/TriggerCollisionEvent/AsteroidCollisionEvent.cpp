#include "AsteroidCollisionEvent.h"

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
		DEBUG_LOG("Asteroid life: " << m_AsteroidLife->GetCurrentLife());
	}
}
