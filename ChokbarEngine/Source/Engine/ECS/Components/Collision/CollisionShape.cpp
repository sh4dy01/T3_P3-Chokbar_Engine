#include "Chokbar.h"
#include "CollisionShape.h"
#include "Engine/PhysicsManager.h"

CollisionShape::CollisionShape()
{
	m_Center = DirectX::XMFLOAT3(0, 0, 0);
}

CollisionShape::~CollisionShape()
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		if (!triggerCollisionEvent) continue;

		delete triggerCollisionEvent;
		triggerCollisionEvent = nullptr;
	}
}

void CollisionShape::CallOnTriggerEnter(CollisionShape* other)
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerEnter(other);
	}
}

void CollisionShape::CallOnTriggerStay(CollisionShape* other)
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerStay(other);
	}
}

void CollisionShape::CallOnTriggerExit(CollisionShape* other)
{
	for (auto triggerCollisionEvent : m_triggerCollisionEvents)
	{
		triggerCollisionEvent->OnTriggerExit(other);
	}
}

void CollisionShape::OnAddedComponent()
{
	Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
	rigidbody->RegisterCollisionShape(this);
}

void CollisionShape::RegisterTriggerCollisionEvent(TriggerCollisionEvent* triggerCollisionEvent)
{
	m_triggerCollisionEvents.push_back(triggerCollisionEvent);
}


