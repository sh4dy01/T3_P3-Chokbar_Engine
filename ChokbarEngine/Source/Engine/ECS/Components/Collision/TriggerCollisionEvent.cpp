#include "Chokbar.h"
#include "TriggerCollisionEvent.h"
#include "Collider.h"

TriggerCollisionEvent::TriggerCollisionEvent()
{

}

TriggerCollisionEvent::~TriggerCollisionEvent()
{
}

void TriggerCollisionEvent::OnAddedComponent()
{
	// Needs to be refactored to be more generic
	auto* m_holder = gameObject->GetComponent<SphereCollider>();

	if (m_holder)
	{
		m_holder->RegisterTriggerCollisionEvent(this);
	}
	else {
		DEBUG_LOG("No collider found on " << gameObject->GetName());
	}
}
