#include "Chokbar.h"
#include "TriggerCollisionEvent.h"

TriggerCollisionEvent::TriggerCollisionEvent()
{

}

TriggerCollisionEvent::~TriggerCollisionEvent()
{
}

void TriggerCollisionEvent::OnAddedComponent()
{
	CollisionShape* m_holder = gameObject->GetComponent<CollisionShape>();

	if (m_holder)
	{
		m_holder->RegisterTriggerCollisionEvent(this);
	}
	else {
		DEBUG_LOG("No collider found on " << gameObject->GetName());
	}
}
