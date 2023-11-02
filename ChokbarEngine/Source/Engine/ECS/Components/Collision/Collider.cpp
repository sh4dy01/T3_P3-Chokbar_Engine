#include "Chokbar.h"
#include "Collider.h"
#include "Rigidbody.h"

Collider::Collider()
	: m_Center(0, 0, 0)
{
}

Collider::~Collider()
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		if (!triggerCollisionEvent) continue;

		delete triggerCollisionEvent;
		triggerCollisionEvent = nullptr;
	}
}

void Collider::CallOnTriggerEnter(Collider* other)
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerEnter(other);
	}
}

void Collider::CallOnTriggerStay(Collider* other)
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerStay(other);
	}
}

void Collider::CallOnTriggerExit(Collider* other)
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerExit(other);
	}
}

void Collider::OnAddedComponent()
{
	Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
	rigidbody->RegisterCollisionShape(this);
}

void Collider::RegisterTriggerCollisionEvent(TriggerCollisionEvent* triggerCollisionEvent)
{
	m_triggerCollisionEvents.push_back(triggerCollisionEvent);
}


