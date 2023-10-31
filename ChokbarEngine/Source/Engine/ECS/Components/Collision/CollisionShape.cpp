#include "Chokbar.h"
#include "CollisionShape.h"
#include "Engine/PhysicsManager.h"

CollisionShape::CollisionShape()
{

}

void CollisionShape::OnAddedComponent()
{
	RigidBody* rigidbody = gameObject->GetComponent<RigidBody>();
	rigidbody->RegisterCollisionShape(this);
}


CollisionShape::ShapeType CollisionShape::GetType() const
{
	return m_type;
}


