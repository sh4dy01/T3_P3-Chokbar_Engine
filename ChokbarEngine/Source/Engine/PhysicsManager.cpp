#include "Chokbar.h"
#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{

}

void PhysicsManager::Update(float deltaTime)
{
}


void PhysicsManager::addCollisionShape(CollisionShape* shape)
{
	m_CollisionShapes.push_back(shape);
}

void PhysicsManager::removeCollisionShape(CollisionShape* shape)
{
	m_CollisionShapes.erase(std::remove(m_CollisionShapes.begin(), m_CollisionShapes.end(), shape), m_CollisionShapes.end());
}

bool PhysicsManager::detectCollision(CollisionShape* shape1, CollisionShape* shape2) const
{
	if (shape1->Intersects(*shape2))
	{
		return true;
	}

	
}
