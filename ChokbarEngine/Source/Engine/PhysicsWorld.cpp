#include "Chokbar.h"
#include "PhysicsWorld.h"


PhysicsWorld::PhysicsWorld()
	: m_gridSize(0), m_cellSize(0.0f)
{

}

PhysicsWorld::PhysicsWorld(int gridSize, float cellSize)
    : m_gridSize(gridSize), m_cellSize(cellSize) 
{

}

PhysicsWorld::~PhysicsWorld() 
{
    for (auto& rigidbody : m_rigidbodies)
    {
		delete rigidbody;
		rigidbody = nullptr;
	}
}

void PhysicsWorld::Update() 
{
	if (m_rigidbodies.size() >= 2)
	{
		for (int i = 0 ; i < m_rigidbodies.size(); i++)
		{
			for (int j = 0; j < m_rigidbodies.size(); j++)
			{
				if (i == j) continue;

				if (DetectCollision(m_rigidbodies[i], m_rigidbodies[j]))
				{
					m_rigidbodies[i]->SetVelocity(XMFLOAT3(0, 0, 0));
					//rigidbody->OnTriggerEnter(otherRigidbody);

					//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " collided with " << m_rigidbodies[j]->gameObject->GetName());
				}
			}
		}
	}
}

void PhysicsWorld::RegisterRigidBody(Rigidbody* rigidbody)
{
    m_rigidbodies.push_back(rigidbody);
}

void PhysicsWorld::RemoveRigidBody(Rigidbody* rigidbody)
{
    m_rigidbodies.erase(std::remove(m_rigidbodies.begin(), m_rigidbodies.end(), rigidbody), m_rigidbodies.end());
}



bool PhysicsWorld::DetectCollision(Rigidbody* rbA, Rigidbody* rbB) const
{
	// si true 
	// rigidbody->setveloc 0
	// OnTriggerEnter

	for (auto& shapeA : rbA->GetAllCollisionShape())
	{
		for (auto& shapeB : rbB->GetAllCollisionShape())
		{
			if (shapeA->GetType() == CollisionShape::ShapeType::Sphere && shapeB->GetType() == CollisionShape::ShapeType::Sphere)
			{
				auto sphereA = (Sphere*)shapeA;
				auto sphereB = (Sphere*)shapeB;

				return AreSpheresColliding(sphereA, sphereB);

			}
			//          else if (shapeA->GetType() == CollisionShape::ShapeType::Sphere && shapeB->GetType() == CollisionShape::ShapeType::Box)
			//          {
				  //		// sphere-box collision
				  //		// TODO
				  //	}
			//          else if (shapeA->GetType() == CollisionShape::ShapeType::Box && shapeB->GetType() == CollisionShape::ShapeType::Sphere)
			//          {
				  //		// box-sphere collision
				  //		// TODO
				  //	}
			//          else if (shapeA->GetType() == CollisionShape::ShapeType::Box && shapeB->GetType() == CollisionShape::ShapeType::Box)
			//        {
				  //		// box-box collision
				  //		// TODO
				  //	}
				  //}

		}
	}



}

bool PhysicsWorld::AreSpheresColliding(Sphere* sphere1, Sphere* sphere2) const
{
	return sphere1->GetBoundingSphere()->Intersects(*sphere2->GetBoundingSphere());
}