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

void PhysicsWorld::RegisterCollider(Collider* collider)
{
	m_RegisteredCollider.push_back(collider);
}

void PhysicsWorld::RemoveCollider(Collider* collider)
{
	if (m_RegisteredCollider.empty()) return;

	std::erase(m_RegisteredCollider, collider);
}

XMFLOAT3 PhysicsWorld::ReduceVelocity(XMFLOAT3& velocity)
{
	velocity.x *= 0.9f;
	velocity.y *= 0.9f;
	velocity.z *= 0.9f;

	return velocity;
}

void PhysicsWorld::Update(float dt)
{
	for (const auto& collider : m_RegisteredCollider)
	{
		Rigidbody* rb = collider->GetAttachedRigidbody();

		if (rb->IsStatic()) continue;

		XMFLOAT3 velocity = rb->GetVelocity();

		if (IsVelocityNull(velocity)) continue;

		XMFLOAT3 fixedVelocity = velocity;


		rb->Move(fixedVelocity);

		rb->SetVelocity(ReduceVelocity(velocity));
	}

	m_timer += dt;

	if (m_timer >= TimeManager::GetFixedTime())
	{
		Coordinator::GetInstance()->FixedUpdateComponents();

		CheckCollision();

		m_timer = 0.0f;
	}
}

bool PhysicsWorld::IsVelocityNull(const XMFLOAT3 velocity)
{
	return	std::abs(velocity.x) <= FLT_EPSILON &&
		std::abs(velocity.y) <= FLT_EPSILON &&
		std::abs(velocity.z) <= FLT_EPSILON;
}

bool PhysicsWorld::IsSameGridPos(const XMINT3 iGridPos, const int iGridSize, XMINT3 jGridPos, int jGridSize)
{
	DEBUG_LOG("Testing " << iGridPos.x << iGridPos.y << iGridPos.z << " and \n " << jGridPos.x << jGridPos.y << jGridPos.z)
	return	std::abs(iGridPos.x - jGridPos.x) < iGridSize + jGridSize &&
			std::abs(iGridPos.y - jGridPos.y) < iGridSize + jGridSize &&
			std::abs(iGridPos.z - jGridPos.z) < iGridSize + jGridSize;
}

void PhysicsWorld::CheckCollision()
{
	for (size_t i = 0; i < m_RegisteredCollider.size(); i++)
	{
		for (size_t j = i + 1; j < m_RegisteredCollider.size(); j++)
		{
			if (!m_RegisteredCollider[i]->gameObject->m_CollisionBitmask.IsLayerInMask(m_RegisteredCollider[j]->gameObject->m_CategoryBitmask.GetLayer()) ||
				!m_RegisteredCollider[j]->gameObject->m_CollisionBitmask.IsLayerInMask(m_RegisteredCollider[i]->gameObject->m_CategoryBitmask.GetLayer())) continue;

			if (IsSameGridPos(
				m_RegisteredCollider[i]->GetAttachedRigidbody()->GetGridPosition(), m_RegisteredCollider[i]->GetGridSize(), 
				m_RegisteredCollider[j]->GetAttachedRigidbody()->GetGridPosition(), m_RegisteredCollider[j]->GetGridSize()) ||
				(m_CurrentCollisionInfo && m_CurrentCollisionInfo->GetState() != Exit))
			{
				Collider* colliderA = m_RegisteredCollider[i];
				Collider* colliderB = m_RegisteredCollider[j];

				Rigidbody* rbA = colliderA->GetAttachedRigidbody();
				Transform& aTransform = *colliderA->transform;

				Rigidbody* rbB = colliderB->GetAttachedRigidbody();
				Transform& bTransform = *colliderB->transform;

				if (AreShapesColliding(colliderA, colliderB))
				{
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

						DEBUG_LOG(rbA->gameObject->GetName() << " continue colliding with " << rbB->gameObject->GetName())

							distance = sqrt(
								pow(aTransform.GetPosition().x - bTransform.GetPosition().x, 2) +
								pow(aTransform.GetPosition().y - bTransform.GetPosition().y, 2) +
								pow(aTransform.GetPosition().z - bTransform.GetPosition().z, 2)
							) - dynamic_cast<SphereCollider*>(colliderA)->GetRadius() + dynamic_cast<SphereCollider*>(colliderB)->GetRadius();

						XMFLOAT3 direction = XMFLOAT3(aTransform.GetPosition().x - bTransform.GetPosition().x, aTransform.GetPosition().y - bTransform.GetPosition().y, aTransform.GetPosition().z - bTransform.GetPosition().z);
						const XMVECTOR vDirection = XMVector3Normalize(XMLoadFloat3(&direction));
						XMStoreFloat3(&direction, vDirection);

						std::abs(direction.x) <= FLT_EPSILON ? 0 : direction.x /= distance / 2;
						std::abs(direction.y) <= FLT_EPSILON ? 0 :  direction.y /= distance / 2;
						std::abs(direction.z) <= FLT_EPSILON ? 0 :  direction.z /= distance / 2;

						rbA->AddVelocity(direction);
						rbB->AddVelocity(XMFLOAT3(-direction.x, -direction.y, -direction.z));

						break;
					case Exit:

						//m_rigidbodies[i]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderB);
						//m_rigidbodies[j]->CallOnCollisionExit(m_CurrentCollisionInfo.ColliderA);

						DEBUG_LOG(m_RegisteredCollider[i]->gameObject->GetName() << " exited collision with " << m_RegisteredCollider[j]->gameObject->GetName())
						std::erase(m_RegisteredCollisionInfos, m_CurrentCollisionInfo);
						DELPTR(m_CurrentCollisionInfo);

						break;
					}
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
