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
	m_RegisteredCollisionInfos.clear();
}

void PhysicsWorld::Update(float dt)
{
	m_timer += dt;

	if (m_timer >= TimeManager::GetFixedTime())
	{
		Coordinator::GetInstance()->FixedUpdateComponents();

		CheckCollision();

		m_timer = 0.0f;
	}

	for (auto& collider : m_RegisteredCollider)
	{
		Rigidbody* rb = collider->GetAttachedRigidbody();

		if (rb->GetVelocity().x == 0 &&
			rb->GetVelocity().y == 0 &&
			rb->GetVelocity().z == 0) continue;

		rb->Move(rb->GetVelocity());

		XMFLOAT3 newVel;
		XMStoreFloat3(&newVel, XMLoadFloat3(&rb->GetVelocity()) * 0.9f);
		rb->SetVelocity(newVel);
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

		XMFLOAT3 iGridPos = m_RegisteredCollider[i]->GetAttachedRigidbody()->GetGridPosition();
		int iGridSize = m_RegisteredCollider[i]->GetGridSize();

		Transform& iTransform = *m_RegisteredCollider[i]->transform;

		for (size_t j = i + 1; j < m_RegisteredCollider.size(); j++)
		{
			XMFLOAT3 jGridPos = m_RegisteredCollider[j]->GetAttachedRigidbody()->GetGridPosition();
			int jGridSize = m_RegisteredCollider[j]->GetGridSize();

			Transform& jTransform = *m_RegisteredCollider[j]->transform;

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


					float distance;

					switch (m_CurrentCollisionInfo->GetState())
					{
					case Enter:

						m_CurrentCollisionInfo->GetColliderA()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderB());
						m_CurrentCollisionInfo->GetColliderB()->CallOnTriggerEnter(m_CurrentCollisionInfo->GetColliderA());

						//distance = sqrt(pow(iGridPos.x - jGridPos.x, 2) + pow(iGridPos.y - jGridPos.y, 2) + pow(iGridPos.z - jGridPos.z, 2)) - ((SphereCollider*)m_RegisteredCollider[i])->GetRadius() + ((SphereCollider*)m_RegisteredCollider[j])->GetRadius();
						

						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " entered in collision with " << m_RegisteredCollider[j]->gameObject->GetName());

						break;
					case Stay:

						//m_rigidbodies[i]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionStay(m_CurrentCollisionInfo.ColliderA);

						//DEBUG_LOG(m_rigidbodies[i]->gameObject->GetName() << " continue colliding with " << m_rigidbodies[j]->gameObject->GetName())

						distance = sqrt(pow(iTransform.GetPosition().x - jTransform.GetPosition().x, 2) + pow(iTransform.GetPosition().y - jTransform.GetPosition().y, 2) + pow(iTransform.GetPosition().z - jTransform.GetPosition().z, 2)) - ((SphereCollider*)m_RegisteredCollider[i])->GetRadius() + ((SphereCollider*)m_RegisteredCollider[j])->GetRadius();

						XMFLOAT3 direction = XMFLOAT3(iTransform.GetPosition().x - jTransform.GetPosition().x, iTransform.GetPosition().y - jTransform.GetPosition().y, iTransform.GetPosition().z - jTransform.GetPosition().z);
						XMVECTOR vDirection = XMVector3Normalize(XMLoadFloat3(&direction));
						XMStoreFloat3(&direction, vDirection);

						direction.x != 0 ? direction.x /= distance / 2 : 0;
						direction.y != 0 ? direction.y /= distance / 2 : 0;
						direction.z != 0 ? direction.z /= distance / 2 : 0;

						m_RegisteredCollider[i]->GetAttachedRigidbody()->AddVelocity(direction);
						m_RegisteredCollider[j]->GetAttachedRigidbody()->AddVelocity(XMFLOAT3(-direction.x, -direction.y, -direction.z));

						break;
					case Exit:

						//m_rigidbodies[i]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " exited collision with " << m_RegisteredCollider[j]->gameObject->GetName());
						std::erase(m_RegisteredCollisionInfos, m_CurrentCollisionInfo);

						break;
					}
				}
		}

		//DEBUG_LOG(std::to_string(temp));
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
	}

	return false;

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
	auto newCollisionInfo = NEW CollisionInfo(sphereA, sphereB);

	m_RegisteredCollisionInfos.push_back(newCollisionInfo);
	m_CurrentCollisionInfo = newCollisionInfo;

	newCollisionInfo = nullptr;
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
