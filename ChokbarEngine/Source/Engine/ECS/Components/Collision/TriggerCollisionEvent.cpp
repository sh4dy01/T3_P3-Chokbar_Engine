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
	auto* m_holder = gameObject->GetComponent<Collider>();

	if (m_holder)
	{
		m_holder->RegisterTriggerCollisionEvent(this);
	}
	else {
		DEBUG_LOG("No collider found on " << gameObject->GetName());
	}
}
