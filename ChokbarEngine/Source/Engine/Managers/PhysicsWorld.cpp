#include "Chokbar.h"
#include "PhysicsWorld.h"

#include "Engine/Engine.h"

using namespace DirectX;

CollisionInfo::CollisionInfo(Collider* colliderA, Collider* colliderB)
	: m_ColliderA(colliderA), m_ColliderB(colliderB), m_State(Enter)
{
}

CollisionInfo::~CollisionInfo()
{
	m_ColliderA = nullptr;
	m_ColliderB = nullptr;
}

void CollisionInfo::UpdateState(CollisionState newState)
{
	m_State = newState;
}

PhysicsWorld::PhysicsWorld()
	: m_CurrentCollisionInfo(nullptr), m_gridSize(0), m_cellSize(0.0f), m_timer(0.0f)
{

}

PhysicsWorld::~PhysicsWorld()
{
	for (auto& rigidbody : m_RegisteredCollider)
	{
		rigidbody = nullptr;
	}

	for (auto& collisionInfo : m_RegisteredCollisionInfos)
	{
		DELPTR(collisionInfo);
	}

	m_RegisteredCollider.clear();
}

void PhysicsWorld::Update(float dt)
{
	m_timer += dt;

	if (m_timer >= TimeManager::GetFixedTime())
	{
		Coordinator::GetInstance()->FixedUpdateComponents();

		CheckCollision();

		for (auto& rigidbody : m_rigidbodies)
		{
			if (rigidbody->GetVelocity().x == 0 &&
				rigidbody->GetVelocity().y == 0 &&
				rigidbody->GetVelocity().z == 0) continue;

			rigidbody->Move(rigidbody->GetVelocity());

			XMFLOAT3 newVel;
			XMStoreFloat3(&newVel, XMLoadFloat3(&rigidbody->GetVelocity()) * 0.4f);
			rigidbody->SetVelocity(newVel);
		}

		m_timer = 0.0f;
	}
}

void PhysicsWorld::RegisterCollider(Collider* collider)
{
	m_RegisteredCollider.push_back(collider);
}

void PhysicsWorld::RemoveCollider(Collider* collider)
{
	if (m_RegisteredCollider.empty()) return;

	std::erase(m_RegisteredCollider, collider);
}

void PhysicsWorld::CheckCollision()
{
	if (m_RegisteredCollider.size() < 2) return;

	for (size_t i = 0; i < m_RegisteredCollider.size(); i++)
	{
		int temp = 0;

		XMFLOAT3 iGridPos = m_RegisteredCollider[i]->GetGridPosition();
		int iGridSize = m_RegisteredCollider[i]->GetAllCollisionShape()[0]->GetGridSize();

		for (size_t j = i + 1; j < m_RegisteredCollider.size(); j++)
		{
			XMFLOAT3 jGridPos = m_rigidbodies[j]->GetGridPosition();
			int jGridSize = m_rigidbodies[j]->GetAllCollisionShape()[0]->GetGridSize();

			if (std::abs(iGridPos.x - jGridPos.x) < iGridSize + jGridSize &&
				std::abs(iGridPos.y - jGridPos.y) < iGridSize + jGridSize &&
				std::abs(iGridPos.z - jGridPos.z) < iGridSize + jGridSize)
			if (AreShapesColliding(m_RegisteredCollider[i], m_RegisteredCollider[j]))
			{
				temp++;


				/*XMFLOAT3 newPos;
				DirectX::XMStoreFloat3(&newPos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_rigidbodies[i]->transform->GetPosition()), DirectX::XMLoadFloat3(&m_rigidbodies[i]->GetVelocity())));
				m_rigidbodies[i]->Move(newPos);

				XMFLOAT3 newPos2;
				DirectX::XMStoreFloat3(&newPos2, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_rigidbodies[j]->transform->GetPosition()), DirectX::XMLoadFloat3(&m_rigidbodies[j]->GetVelocity())));
				m_rigidbodies[j]->Move(newPos2);*/

				if (CheckCollisionShapes(m_rigidbodies[i], m_rigidbodies[j]))
				{
					float distance;

				case Enter:

					switch (m_CurrentCollisionInfo->GetState())
					{
					case Enter:

						m_CurrentCollisionInfo->GetColliderA()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderB());
						m_CurrentCollisionInfo->GetColliderB()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderA());

						distance = sqrt(pow(iGridPos.x - jGridPos.x, 2) + pow(iGridPos.y - jGridPos.y, 2) + pow(iGridPos.z - jGridPos.z, 2)) - ((SphereCollider*)m_rigidbodies[i]->GetAllCollisionShape()[0])->GetRadius() + ((SphereCollider*)m_rigidbodies[j]->GetAllCollisionShape()[0])->GetRadius();
					DEBUG_LOG(m_CurrentCollisionInfo->GetColliderA()->gameObject->GetName() << " entered in collision with " << m_CurrentCollisionInfo->GetColliderB()->gameObject->GetName())

						break;
				case Stay:

						XMFLOAT3 direction = XMFLOAT3(iGridPos.x - jGridPos.x, iGridPos.y - jGridPos.y, iGridPos.z - jGridPos.z);
						XMVECTOR vDirection = XMVector3Normalize(XMLoadFloat3(&direction));
						XMStoreFloat3(&direction, vDirection);

						direction.x /= distance / 2;
						direction.y /= distance / 2;
						direction.z /= distance / 2;

						m_rigidbodies[i]->AddVelocity(direction);
						m_rigidbodies[j]->AddVelocity(XMFLOAT3(-direction.x, -direction.y, -direction.z));

						//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " entered in collision with " << m_rigidbodies[j]->gameObject->GetName())

						break;
					case Stay:

						//m_rigidbodies[i]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderA);

						//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " continue colliding with " << m_rigidbodies[j]->gameObject->GetName())

						break;
					case Exit:

						//m_rigidbodies[i]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderA);

						/*DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " exited collision with " << m_rigidbodies[j]->gameObject->GetName())
							std::erase(m_RegisteredCollisionInfos, m_CurrentCollisionInfo);*/

						break;
					}

					//m_rigidbodies[i]->SetVelocity(XMFLOAT3(0, 0, 0));

					//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " collided with " << m_rigidbodies[j]->gameObject->GetName());
					DEBUG_LOG(m_CurrentCollisionInfo->GetColliderA()->gameObject->GetName() << " exited collision with " << m_CurrentCollisionInfo->GetColliderB()->gameObject->GetName())
						std::erase(m_RegisteredCollisionInfos, m_CurrentCollisionInfo);

					break;

				}
			}
		}

		DEBUG_LOG(std::to_string(temp));
	}
}

bool PhysicsWorld::AreShapesColliding(Collider* shapeA, Collider* shapeB)
{
	if (shapeA->GetType() == Collider::ShapeType::Sphere && shapeB->GetType() == Collider::ShapeType::Sphere)
	{
		const auto sphereA = dynamic_cast<SphereCollider*>(shapeA);
		const auto sphereB = dynamic_cast<SphereCollider*>(shapeB);

		if (AreSpheresColliding(sphereA, sphereB))
		{
			if (shapeA->GetType() == Collider::ShapeType::Sphere && shapeB->GetType() == Collider::ShapeType::Sphere)
			{
				const auto sphereA = dynamic_cast<SphereCollider*>(shapeA);
				const auto sphereB = dynamic_cast<SphereCollider*>(shapeB);

				if (AreSpheresColliding(sphereA, sphereB))
				{
					HandleCollision(sphereA, sphereB);

					return true;
				}
				// If there is a collision already registered
				else if (!m_RegisteredCollisionInfos.empty())
				{
					// We check if the collision is still occuring and set it to exit
					if (const auto collisionInfo = GetCollisionInfo(sphereA, sphereB))
					{
						collisionInfo->UpdateState(Exit);
						m_CurrentCollisionInfo = collisionInfo;

						// return true to handle the exit trigger
						return true;
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

			return true;
		}

		/*
		// If there is a collision already registered
		else if (!m_RegisteredCollisionInfos.empty())
		{
			// We check if the collision is still occuring and set it to exit
			if (const auto collisionInfo = GetCollisionInfo(sphereA, sphereB))
			{
				collisionInfo->UpdateState(Exit);
				m_CurrentCollisionInfo = collisionInfo;

				// return true to handle the exit trigger
			}
		}	*/

	}

	return false;

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

void PhysicsWorld::HandleCollision(Collider* const sphereA, Collider* const sphereB)
{
	const auto collisionInfo = GetCollisionInfo(sphereA, sphereB);

	if (!collisionInfo)
	{
		CreateNewCollisionInfo(sphereA, sphereB);
	}
	else
	{
		collisionInfo->UpdateState(Stay);
		m_CurrentCollisionInfo = collisionInfo;
	}
}

void PhysicsWorld::CreateNewCollisionInfo(Collider* const sphereA, Collider* const sphereB)
{
	const auto newCollisionInfo = NEW CollisionInfo(sphereA, sphereB);

	m_RegisteredCollisionInfos.push_back(newCollisionInfo);
	m_CurrentCollisionInfo = newCollisionInfo;
}

CollisionInfo* PhysicsWorld::GetCollisionInfo(const Collider* sphereA, const Collider* sphereB) const
{
	for (const auto& collisionInfo : m_RegisteredCollisionInfos)
	{
		if (collisionInfo->GetColliderA() == sphereA && collisionInfo->GetColliderB() == sphereB)
		{
			return collisionInfo;
		}
	}

	return nullptr;
}

bool PhysicsWorld::AreSpheresColliding(SphereCollider* sphereA, SphereCollider* sphereB) const
{
	// Load A.
	XMVECTOR vCenterA = DirectX::XMVectorAdd(XMLoadFloat3(&sphereA->GetCenter()), XMLoadFloat3(&sphereA->transform->GetPosition()));
	XMVECTOR vRadiusA = DirectX::XMVectorReplicate(sphereA->GetRadius());

	// Load B.
	XMVECTOR vCenterB = DirectX::XMVectorAdd(XMLoadFloat3(&sphereB->GetCenter()), XMLoadFloat3(&sphereB->transform->GetPosition()));
	XMVECTOR vRadiusB = DirectX::XMVectorReplicate(sphereB->GetRadius());

	// Distance squared between centers.    
	XMVECTOR Delta = DirectX::XMVectorSubtract(vCenterB, vCenterA);
	XMVECTOR DistanceSquared = DirectX::XMVector3LengthSq(Delta);

	// Sum of the radii squared.
	XMVECTOR RadiusSquared = DirectX::XMVectorAdd(vRadiusA, vRadiusB);
	RadiusSquared = DirectX::XMVectorMultiply(RadiusSquared, RadiusSquared);

	return DirectX::XMVector3LessOrEqual(DistanceSquared, RadiusSquared);
}
