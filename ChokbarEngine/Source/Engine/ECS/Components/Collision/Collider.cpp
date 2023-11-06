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
		DELPTR(triggerCollisionEvent);
	}
}

void Collider::CallOnTriggerEnter(Collider* other) const
{
	for (const auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerEnter(other);
	}
}

void Collider::CallOnTriggerStay(Collider* other) const
{
	for (const auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerStay(other);
	}
}

void Collider::CallOnTriggerExit(Collider* other) const
{
	for (const auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerExit(other);
	}
}

void Collider::OnAddedComponent()
{
	const auto rigidbody = gameObject->GetComponent<Rigidbody>();
	m_AttachedRigidbody = rigidbody;

	if (rigidbody != nullptr)
		Engine::GetPhysicsWorld()->RegisterCollider(this);
	else 
		DEBUG_LOG("Collider component added to a game object without a rigidbody component")
}

void Collider::OnRemovedComponent()
{
	Engine::GetPhysicsWorld()->RemoveCollider(this);
}

void Collider::RegisterTriggerCollisionEvent(TriggerCollisionEvent* triggerCollisionEvent)
{
	m_triggerCollisionEvents.push_back(triggerCollisionEvent);
}


