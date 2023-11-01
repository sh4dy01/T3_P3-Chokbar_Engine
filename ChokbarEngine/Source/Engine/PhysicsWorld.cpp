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

void PhysicsWorld::Update(float dt)
{
	m_updateRate = 0.02f;
	m_timer += dt;
	if (m_timer >= m_updateRate)
	{
		CheckCollision();
		m_timer = 0.0f;
	}
}

void PhysicsWorld::RegisterRigidBody(Rigidbody* rigidbody)
{
	m_rigidbodies.push_back(rigidbody);
}

void PhysicsWorld::RemoveRigidBody(Rigidbody* rigidbody)
{
	std::erase(m_rigidbodies, rigidbody);
}

void PhysicsWorld::CheckCollision()
{
	if (m_rigidbodies.size() >= 2)
	{
		for (size_t i = 0; i < m_rigidbodies.size(); i++)
		{
			for (size_t j = 0; j < m_rigidbodies.size(); j++)
			{
				if (i == j) continue;

				if (DetectCollision(m_rigidbodies[i], m_rigidbodies[j]))
				{
					m_rigidbodies[i]->SetVelocity(XMFLOAT3(0, 0, 0));
					// m_rigidbodies[i]->CallOnCollisionEnter();

					//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " collided with " << m_rigidbodies[j]->gameObject->GetName());
				}
			}
		}
	}
}

bool PhysicsWorld::DetectCollision(Rigidbody* rbA, Rigidbody* rbB)
{
	// si true 
	// rigidbody->setveloc 0
	// OnTriggerEnter

	for (const auto& shapeA : rbA->GetAllCollisionShape())
	{
		for (const auto& shapeB : rbB->GetAllCollisionShape())
		{
			if (shapeA->GetType() == CollisionShape::ShapeType::Sphere && shapeB->GetType() == CollisionShape::ShapeType::Sphere)
			{
				auto sphereA = static_cast<Sphere*>(shapeA);
				auto sphereB = static_cast<Sphere*>(shapeB);

				if (AreSpheresColliding(sphereA, sphereB))
				{
					m_collisionShapeA = sphereA;
					m_collisionShapeB = sphereB;

					return true;
				}
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