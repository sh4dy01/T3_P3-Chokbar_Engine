#include "PlayerCollisionEvent.h"

PlayerCollisionEvent::~PlayerCollisionEvent()
{
	m_PlayerLife = nullptr;
}

void PlayerCollisionEvent::OnAddedComponent()
{
	TriggerCollisionEvent::OnAddedComponent();

	m_PlayerLife = gameObject->GetComponent<PlayerLife>();
}

void PlayerCollisionEvent::OnTriggerEnter(Collider* other)
{
	if (other->gameObject->m_CategoryBitmask.GetLayer() == ASTEROID)
	{
		DEBUG_LOG("Player")
		m_PlayerLife->RemoveLife(5);
	}
}
