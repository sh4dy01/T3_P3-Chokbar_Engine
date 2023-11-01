#include "Chokbar.h"
#include "PhysicsWorld.h"


PhysicsWorld::PhysicsWorld()
	: m_gridSize(0), m_cellSize(0.0f), UPDATE_RATE(0.02f), m_timer(0.0f)
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

	for (auto& collisionInfo : m_RegisteredCollisionInfos)
	{
		delete collisionInfo;
		collisionInfo = nullptr;
	}
}

void PhysicsWorld::Update(float dt)
{
	m_timer += dt;

	if (m_timer >= UPDATE_RATE)
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
			for (size_t j = 1; j < m_rigidbodies.size(); j++)
			{
				if (CheckCollisionShape(m_rigidbodies[i], m_rigidbodies[j]))
				{
					switch (m_CurrentCollisionInfo->State)
					{
					case Enter:

						m_rigidbodies[i]->CallOnCollisionEnter(m_CurrentCollisionInfo->ColliderB);
						m_rigidbodies[j]->CallOnCollisionEnter(m_CurrentCollisionInfo->ColliderA);

						DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " entered in collision with " << m_rigidbodies[j]->gameObject->GetName())

						break;
					case Stay:

						//m_rigidbodies[i]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " continue colliding with " << m_rigidbodies[j]->gameObject->GetName())

						break;
					case Exit:

						//m_rigidbodies[i]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " exited collision with " << m_rigidbodies[j]->gameObject->GetName())

						break;
					}
					
					//m_rigidbodies[i]->SetVelocity(XMFLOAT3(0, 0, 0));

					//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " collided with " << m_rigidbodies[j]->gameObject->GetName());
				}
			}
		}
	}
}

bool PhysicsWorld::CheckCollisionShape(Rigidbody* rbA, Rigidbody* rbB)
{
	for (const auto& shapeA : rbA->GetAllCollisionShape())
	{
		for (const auto& shapeB : rbB->GetAllCollisionShape())
		{
			if (shapeA->GetType() == CollisionShape::ShapeType::Sphere && shapeB->GetType() == CollisionShape::ShapeType::Sphere)
			{
				const auto sphereA = dynamic_cast<Sphere*>(shapeA);
				const auto sphereB = dynamic_cast<Sphere*>(shapeB);

				if (AreSpheresColliding(sphereA, sphereB))
				{
					HandleCollision(sphereA, sphereB);

					return true;
				}
				else if (!m_RegisteredCollisionInfos.empty())
				{
					if (const auto collisionInfo = GetCollisionInfo(sphereA, sphereB))
					{
						collisionInfo->State = Exit;
						m_CurrentCollisionInfo = collisionInfo;
					}
				}	

				return false;
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

	return false;
}

void PhysicsWorld::HandleCollision(CollisionShape* const sphereA, CollisionShape* const sphereB)
{
	const auto collisionInfo = GetCollisionInfo(sphereA, sphereB);

	if (!collisionInfo)
	{
		CreateNewCollisionInfo(sphereA, sphereB);
	}
	else
	{
		collisionInfo->State = Stay;
		m_CurrentCollisionInfo = collisionInfo;
	}
}

void PhysicsWorld::CreateNewCollisionInfo(CollisionShape* const sphereA, CollisionShape* const sphereB)
{
	const auto newCollisionInfo = new CollisionInfo(sphereA, sphereB);

	m_RegisteredCollisionInfos.push_back(newCollisionInfo);
	m_CurrentCollisionInfo = newCollisionInfo;
}

CollisionInfo* PhysicsWorld::GetCollisionInfo(const CollisionShape* sphereA, const CollisionShape* sphereB) const
{
	for (const auto& collisionInfo : m_RegisteredCollisionInfos)
	{
		if (collisionInfo->ColliderA == sphereA && collisionInfo->ColliderB == sphereB)
		{
			return collisionInfo;
		}
	}

	return nullptr;
}

bool PhysicsWorld::AreSpheresColliding(Sphere* sphere1, Sphere* sphere2) const
{
	return sphere1->GetBoundingSphere()->Intersects(*sphere2->GetBoundingSphere());
}