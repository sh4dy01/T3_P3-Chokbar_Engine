#include "Chokbar.h"
#include "CollisionShape.h"
#include "Engine/PhysicsManager.h"

CollisionShape::CollisionShape()
{

}

void CollisionShape::OnAddedComponent()
{
	Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
	rigidbody->RegisterCollisionShape(this);
}


CollisionShape::ShapeType CollisionShape::GetType() const
{
	return m_type;
}


